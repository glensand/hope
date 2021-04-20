/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <cstdint>
#include <new>
#include <cassert>
#include <limits>
#include <new>

namespace hope::memory{

    /**
     * \brief Low level chunk allocator, is made such as tricky linked list
     * first byte of the chunks block stores position of the next available element, thus we try to avoid
     * extra memory utilization, also chunk knows nothing about it's block size, therefore alloc and dealloc methods looks so strange
     * (their signature contain related params...)
     * but it is worth remembering that chunk can't allocate blocks with different sizes
     */
    struct chunk final{

            /**
         * \brief I decide to do not use RAII principle, to avoid any explicit constructors
         *   chunk should be as simple struct as possible, btw it is lowest level in allocation hierarchy
         * \param block_size size of single block to be stored at the chunk; this is only one lock size
         *      which is available for allocation 
         * \param blocks_count count of chunk's blocks to be allocated, value in range of [0, 255]
         * this is most appropriate interval to avoid memory alignment falls
         * (maximum available index of chunk block is 255 due to byte size)
         */
            void init(std::size_t block_size, uint8_t blocks_count) noexcept;

            /**
         * \brief Allocation of chunks block, block_size is passed throw methods parameter, because
         * it is too expensive to store this value at the chunk struct (due to the allocators hierarchy, see fixed_allocator.h for more details)
         * this method extract first available block from linked list, and do other related work, it is easy to understand 
         * \param block_size - size of block to be allocated
         * \return pointer to the desired memory segment
         */
            [[nodiscard]] void* allocate(std::size_t block_size) noexcept;

            /**
         * \brief freed passed memory witch is related by the passed pointer
         * \param ptr pointer to the memory block to be allocated, value should be in range of [data, data + block_size * blocks_count]
         * in other words this memory should be recently allocated from this chunk
         * \param block_size size of block to be freed
         */
            void deallocate(void* ptr, std::size_t block_size) noexcept;

            /**
         * \brief checks if deallocation of passed ptr with passed value is available,
         * otherwise we just cough memory corruption ot other invalid activity
         * \param ptr pointer to the block to be deallocated
         * \param block_size size of deallocating block  
         * \return true id deallocation may be successful, otherwise false
         */
            [[nodiscard]] bool is_deallocation_valid(void* ptr, std::size_t block_size) const noexcept;

            /**
         * \brief check if allocation of block currently available 
         * \param block_size size of block to be allocated
         * \return true if allocation is possible
         */
            [[nodiscard]] bool is_allocation_valid(std::size_t block_size) const noexcept;

        uint8_t*    data{ nullptr };        // Pointer to thr very beginning of the memory pool
        uint8_t     free_blocks_count{ 0 }; // Count of currently available blocks count, variables type is linked with alignment and other complicated things
        uint8_t     first_free_block{ 0 };  // Position of first available block
    };

    inline void chunk::init(std::size_t block_size, uint8_t blocks_count) noexcept {
        data = new(std::nothrow) uint8_t[block_size * std::size_t(blocks_count)];
        assert(data != nullptr);
        free_blocks_count = blocks_count;
        auto* begin = data;
        for (uint8_t i{ 0 }; i < blocks_count; ++i, begin += block_size)
            *begin = i + 1;
    }

    inline void* chunk::allocate(std::size_t block_size) noexcept {
        assert(is_allocation_valid(block_size));
        auto* const result = data + std::size_t(first_free_block) * block_size;
        first_free_block = *result;
        --free_blocks_count;
        return result;
    }

    inline void chunk::deallocate(void* ptr, std::size_t block_size) noexcept {
        assert(is_deallocation_valid(ptr, block_size));
        const auto byte_index = std::size_t(static_cast<uint8_t*>(ptr) - data);
        const auto block_index = uint8_t(byte_index / block_size);
        data[byte_index] = first_free_block;
        first_free_block = block_index;
        ++free_blocks_count;
    }

    inline bool chunk::is_deallocation_valid(void* ptr, std::size_t block_size) const noexcept {
        return data != nullptr
            && ptr != nullptr
            && free_blocks_count < std::numeric_limits<uint8_t>::max()
            && block_size > 0
            && (static_cast<uint8_t*>(ptr) - data) % block_size == 0;
    }

    inline bool chunk::is_allocation_valid(std::size_t block_size) const noexcept {
        return data != nullptr
            && free_blocks_count > 0
            && block_size > 0;
    }
}
