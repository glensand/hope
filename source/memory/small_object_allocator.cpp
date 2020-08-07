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
            assert(m_is_initialized);
            const auto alloc = find_allocator(size);
            assert(alloc);
            alloc->deallocate(ptr);
        }
    }

    void* small_object_allocator::allocate(std::size_t size) noexcept{
        assert(m_is_initialized);
        if(size > m_max_object_size) [[unlikely]]
            return new uint8_t[size];
        auto alloc = find_allocator(size);
        if (alloc == nullptr) [[unlikely]]
            alloc = create_allocator(size);
        assert(alloc != nullptr);
        return alloc->allocate();
    }

    void small_object_allocator::initialize(std::size_t chunk_size, std::size_t max_object_size) noexcept {
        m_chunk_size = chunk_size;
        m_max_object_size = max_object_size;
        m_is_initialized = true;
    }

    fixed_allocator* small_object_allocator::find_allocator(std::size_t size) noexcept {
        const auto allocIt = std::find_if(std::begin(m_allocator_list), std::end(m_allocator_list),
            [=](const auto& allocator) {
                return allocator.block_size() == size;
            });
        return allocIt != std::end(m_allocator_list) ? &*allocIt : nullptr;
    }

    fixed_allocator* small_object_allocator::create_allocator(std::size_t size) noexcept {
        const auto forwardIt = std::adjacent_find(std::begin(m_allocator_list), std::end(m_allocator_list),
            [=](const auto& allocator1, const auto& allocator2) {
                return allocator1.block_size() < size && allocator2.block_size() > size;
            });
        return &*m_allocator_list.emplace(forwardIt, m_chunk_size, size);
    }
}
