/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "small_object_allocator.h"
#include <algorithm>

namespace hope::memory {

    small_object_allocator& small_object_allocator::instance() {
        static small_object_allocator instance;
        return instance;
    }

    void small_object_allocator::deallocate(void* ptr, std::size_t size) noexcept {
        if (size > m_max_object_size) [[unlikely]] {
            delete[] static_cast<uint8_t*>(ptr);
        } else [[likely]] {
            const auto alloc = find_allocator(size);
            assert(alloc);
            alloc->deallocate(ptr);
        }
    }

    void* small_object_allocator::allocate(std::size_t size) noexcept{
        if(size > m_max_object_size) [[unlikely]] {
            try {
                return new uint8_t[size];
            }
            catch (const std::bad_alloc& ex) {
                assert(false);
                return nullptr;
            }
        }
        auto alloc = find_allocator(size);
        if (alloc == nullptr) [[unlikely]]
            alloc = create_allocator(size);
        assert(alloc != nullptr);
        return alloc->allocate();
    }

    fixed_allocator* small_object_allocator::find_allocator(std::size_t size) noexcept {
        const auto allocIt = std::find_if(std::begin(m_allocator_list), std::end(m_allocator_list),
            [=](const auto& allocator) {
                return allocator.block_size() == size;
            });
        return allocIt != std::end(m_allocator_list) ? &*allocIt : nullptr;
    }

    fixed_allocator* small_object_allocator::create_allocator(std::size_t size) noexcept {
        const auto forwardIt = std::find_if(std::begin(m_allocator_list), std::end(m_allocator_list),
            [=](const auto& allocator) {
                return allocator.block_size() > size;
            });
        const auto isEnd = forwardIt == std::end(m_allocator_list);
        return &*m_allocator_list.emplace(forwardIt, m_chunk_size, size);
    }

    void small_object_allocator::clear() {
        m_allocator_list.clear();
    }

    small_object_allocator::~small_object_allocator() {
        clear();
    }
}
