//------------------------------------------------------------------------------
// fixed_allocator.h
// 
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 5.08.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "chunk.h"
#include <vector>

namespace hope::memory {

    class fixed_allocator final {
        using chunk_list_t = std::vector<chunk>;
    public:
        fixed_allocator(uint8_t max_blocks_count, std::size_t block_size) noexcept;
        fixed_allocator(fixed_allocator&& rhs) noexcept;
        fixed_allocator& operator=(fixed_allocator&& rhs) noexcept;

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

        std::size_t         m_block_size;       // size of single block in the chunk
        uint8_t             m_blocks_in_chunk;  // count of blocks, contains in one chunk
        chunk_list_t        m_chunk_list;       // list of allocated chunks
        chunk*              m_last_allocated{ nullptr };    // last allocation was held in this chunk, cannot be nullptr
        chunk*              m_last_deallocated{ nullptr };  // last deallocation was held in this chunk, cannot be nullptr
        chunk*              m_free_block{ nullptr };        // completely deallocated block, is used to recycle deallocated blocks and avoid new\delete utilization
    };

    inline fixed_allocator::fixed_allocator(uint8_t max_blocks_count, std::size_t block_size) noexcept:
        m_block_size(block_size)
        , m_blocks_in_chunk(max_blocks_count)
    {
        try {
            m_chunk_list.reserve(max_blocks_count);
            // thus, we remove one extra compare 
            m_chunk_list.push_back(create_new_chunk());
            m_last_allocated = &m_chunk_list.front();
            m_last_deallocated = m_last_allocated;
        } catch(const std::exception& ex) {
            assert(false);
        }
    }

    inline fixed_allocator::fixed_allocator(fixed_allocator&& rhs) noexcept
        : m_block_size(rhs.m_block_size)
        , m_blocks_in_chunk(rhs.m_blocks_in_chunk)
        , m_chunk_list(std::move(rhs.m_chunk_list))
        , m_last_allocated(rhs.m_last_allocated)
        , m_last_deallocated(rhs.m_last_deallocated)
    {
        // NOTE:: some fields of rhs value especially was not assign to nullptr
        // due to performance optimization...
    }

    inline fixed_allocator& fixed_allocator::operator=(fixed_allocator&& rhs) noexcept {
        m_block_size = rhs.m_block_size;
        m_blocks_in_chunk = rhs.m_blocks_in_chunk;
        m_chunk_list = std::move(rhs.m_chunk_list);
        m_last_allocated = rhs.m_last_allocated;
        m_last_deallocated = rhs.m_last_deallocated;
        // NOTE:: some fields of rhs value especially was not assign to nullptr
        // due to performance optimization...
        return *this;
    }

    inline void* fixed_allocator::allocate() noexcept {
        if (m_last_allocated->free_blocks_count == 0
            && !update_alloc_chunk()) [[unlikely]] {
            try {
                m_chunk_list.push_back(create_new_chunk());
            } catch (const std::exception& ex) {
                assert(false);
                return nullptr;
            }    
            m_last_allocated = &m_chunk_list.back();
        }
        return m_last_allocated->allocate(m_block_size);
    }

    inline void fixed_allocator::deallocate(void* ptr) noexcept {
        if (!can_be_deallocated(*m_last_deallocated, ptr)) [[likely]]
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
        if(m_free_block != nullptr) {
            m_last_allocated = m_free_block;
            m_free_block = nullptr;
            return true;
        }
        const auto chunk_it = std::find_if(std::begin(m_chunk_list), std::end(m_chunk_list), 
            [=](const auto& ch) {
                        return ch.free_blocks_count > 0;
        });
        const bool isFound = chunk_it != std::end(m_chunk_list);
        if (isFound) [[likely]]
            m_last_allocated = &*chunk_it;
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
        if(m_free_block != nullptr){
            const std::size_t dealloc_block_pos = (m_last_deallocated - m_chunk_list.data()) / sizeof(chunk);
            const auto begin = std::begin(m_chunk_list) + dealloc_block_pos;
            m_chunk_list.erase(begin);
            m_last_deallocated = &m_chunk_list.front();
        } else {
            m_free_block = m_last_deallocated;
        }
    }
}
