/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/memory/small_object/chunk.h"
#include <vector>
#include <algorithm>

namespace hope::memory {

    class fixed_allocator final {
        using chunk_list = std::vector<chunk>;
    public:
        fixed_allocator(uint8_t max_blocks_count, std::size_t block_size) noexcept;
        fixed_allocator(fixed_allocator&& rhs) noexcept;
        fixed_allocator& operator=(fixed_allocator&& rhs) noexcept;
        ~fixed_allocator() noexcept;

        /**
         * \brief to avoid extra or unsafe copying we have to explicitly delete copy constructor and copy assign operator
         * also, we should create explicit noexcept move constructor and copy assign operator
         * \param rhs 
         */
        fixed_allocator(const fixed_allocator& rhs) = delete;
        fixed_allocator& operator=(const fixed_allocator& rhs) = delete;

        /**
         * \brief allocate block with m_block_size size
         */
        void* allocate() noexcept;

        /**
         * \brief try to deallocate passed pointer, if it were allocated by another memory pool
         * in debug this caused assert failure
         * in realize mode this causes undefined behavior
         */
        void deallocate(void* ptr) noexcept;

        /**
         * \brief returns size of block, witch may be allocated
         */
        [[nodiscard]] std::size_t block_size() const noexcept;
    private:
        /**
         * \brief try to find chunk in witch is owner of this portion of memory
         * if cannot find fails with assert(false) in debug, caused ub in release
         * \param ptr 
         */
        void update_dealloc_chunk(void* ptr) noexcept;

        /**
         * \brief find chunk with free blocks and update m_last_allocated member if it was found
         * \return if m_last_allocated was updated
         */
        [[nodiscard]] bool update_alloc_chunk() noexcept;

        /**
         * \brief check if this portion of memory can be deleted by this chunk
         */
        [[nodiscard]] bool can_be_deallocated(const chunk& chunk, void* ptr) const noexcept;

        /**
         * \brief creates new chunk and update m_last_allocated field
         * \return 
         */
        [[nodiscard]] chunk create_new_chunk() const noexcept;

        /**
         * \brief checks a deallocated chunk, if there are two free chunks in the system at the same time, deletes one of them
         */
        void update_free_chunk() noexcept;

        /**
         * \brief Takes resources from the passed object. Used in the move constructor and the move assign operator
         * \param rhs resource donor object
         */
        void steal_resources(fixed_allocator&& rhs) noexcept;

        std::size_t         m_block_size;                   // size of single block in the chunk
        chunk_list          m_chunk_list;                   // list of allocated chunks
        chunk*              m_last_allocated{ nullptr };    // last allocation was held in this chunk, cannot be nullptr
        chunk*              m_last_deallocated{ nullptr };  // last deallocation was held in this chunk, cannot be nullptr
        chunk*              m_free_block{ nullptr };        // completely deallocated block, is used to recycle deallocated blocks and avoid new\delete utilization
        uint8_t             m_blocks_in_chunk;              // count of blocks, contains in one chunk
    };

    inline fixed_allocator::fixed_allocator(uint8_t max_blocks_count, std::size_t block_size) noexcept
        : m_block_size(block_size)
        , m_blocks_in_chunk(max_blocks_count)
    {
        try {
            // thus, we remove one extra compare 
            m_chunk_list.push_back(create_new_chunk());
            m_last_allocated = &m_chunk_list.front();
        } catch(const std::exception&) {
            assert(false);
        }
    }

    inline fixed_allocator::fixed_allocator(fixed_allocator&& rhs) noexcept {  // NOLINT(cppcoreguidelines-pro-type-member-init)
        steal_resources(std::move(rhs));
    }

    inline fixed_allocator& fixed_allocator::operator=(fixed_allocator&& rhs) noexcept {
        steal_resources(std::move(rhs));
        return *this;
    }

    inline fixed_allocator::~fixed_allocator() noexcept {
        for (auto& chunk : m_chunk_list)
            delete[] chunk.data;
    }

    inline void* fixed_allocator::allocate() noexcept {
        if (m_last_allocated->free_blocks_count == 0
            && !update_alloc_chunk()) {
            try {
                m_chunk_list.push_back(create_new_chunk());
            } catch (const std::exception&) {
                assert(false);
                return nullptr;
            }    
            m_last_allocated = &m_chunk_list.back();
            // If std::vector were reallocated m_last_deallocated could be invalid
            // i decide to assign this var to the middle of vec is not bad idea
            m_last_deallocated = &m_chunk_list[m_chunk_list.size() / 2];
        }
        return m_last_allocated->allocate(m_block_size);
    }

    inline void fixed_allocator::deallocate(void* ptr) noexcept {
        if (m_last_deallocated == nullptr 
            || !can_be_deallocated(*m_last_deallocated, ptr))
            update_dealloc_chunk(ptr);
        m_last_deallocated->deallocate(ptr, m_block_size);
        update_free_chunk();
    }

    inline std::size_t fixed_allocator::block_size() const noexcept {
        return m_block_size;
    }

    inline void fixed_allocator::update_dealloc_chunk(void* ptr) noexcept {
        const auto chunk_it = std::find_if(std::begin(m_chunk_list), std::end(m_chunk_list),
            [=](const auto& ch) {
                        return can_be_deallocated(ch, ptr);
        });
        assert(chunk_it != std::end(m_chunk_list));
        m_last_deallocated = &*chunk_it;
    }

    inline bool fixed_allocator::update_alloc_chunk() noexcept {
        const auto chunk_it = std::find_if(std::begin(m_chunk_list), std::end(m_chunk_list), 
            [=](const auto& ch) {
                        return ch.free_blocks_count > 0;
        });
        const bool isFound = chunk_it != std::end(m_chunk_list);
        if (isFound)
        {
            m_last_allocated = &*chunk_it;
            if (m_free_block == m_last_allocated)
                m_free_block = nullptr;
        }
        return isFound;
    }

    inline bool fixed_allocator::can_be_deallocated(const chunk& chunk, void* ptr) const noexcept {
        return ptr >= chunk.data && ptr < chunk.data + m_block_size * m_blocks_in_chunk;
    }

    inline chunk fixed_allocator::create_new_chunk() const noexcept {
        chunk ch;
        ch.init(m_block_size, m_blocks_in_chunk);
        return ch;
    }

    inline void fixed_allocator::update_free_chunk() noexcept {
        if (m_last_deallocated->free_blocks_count != m_blocks_in_chunk)
            return;
        if(m_free_block != nullptr && m_chunk_list.size() > 1){
            const std::size_t dist = m_last_deallocated - m_chunk_list.data();
            const auto begin = std::begin(m_chunk_list) + dist;
            delete[] begin->data;
            m_chunk_list.erase(begin);
            m_last_allocated = m_last_deallocated = &m_chunk_list[m_chunk_list.size() / 2];
        } else if (m_free_block == nullptr){
            m_free_block = m_last_deallocated;
        }
    }

    inline void fixed_allocator::steal_resources(fixed_allocator&& rhs) noexcept {
        m_block_size = rhs.m_block_size;
        m_blocks_in_chunk = rhs.m_blocks_in_chunk;
        m_chunk_list = std::move(rhs.m_chunk_list);
        assert(rhs.m_chunk_list.empty());
        m_last_allocated = rhs.m_last_allocated;
        m_last_deallocated = rhs.m_last_deallocated;
        m_free_block = rhs.m_free_block;
        rhs.m_free_block = rhs.m_last_allocated = rhs.m_last_deallocated = nullptr;
    }
}
