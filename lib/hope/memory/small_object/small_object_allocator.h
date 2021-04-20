/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/memory/small_object/fixed_allocator.h"
#include "hope/memory/small_object/config.h"

namespace hope::memory {

    /**
     * \brief singleton, is used to hold list with fixed allocators of proper sizes
     */
    class small_object_allocator final {
        using allocator_list = std::vector<fixed_allocator>;
    public:
        small_object_allocator(const small_object_allocator&) = delete;
        small_object_allocator(small_object_allocator&&) = delete;
        small_object_allocator& operator=(const small_object_allocator&) = delete;
        small_object_allocator& operator=(small_object_allocator&&) = delete;

        /**
         * \brief just return instance of singleton
         * NOTE: before use this class, u have to initialize its instance, see small_object_allocator::initialize
         */
        static small_object_allocator& instance();

        /**
         * \brief try to deallocate passed pointer to object
         * since allocator is stateful, you cannot deallocate objects, which were allocated by this class
         * params meaning are trivial
         */
        void deallocate(void* ptr, std::size_t size) noexcept;

        /**
         * \brief try to allocate object, if passed size greater than m_max_object_size, operator new will be called
         * params meaning are trivial
         */
        [[nodiscard]] void* allocate(std::size_t size) noexcept;

    private:
        /**
         * \brief Try to find proper allocator (which can allocate objects with passed size);
         * if allocator were not found return nullptr
         * \param size - size :0 of block to be allocated
         * \return found allocator or (NOTE!) nullptr
         */
        fixed_allocator* find_allocator(std::size_t size) noexcept;

        /**
         * \brief create allocator which might allocate objects with passed size
         * \param size - the size of the object for which the allocator will be created
         * \return proper allocator (can not be nullptr)
         */
        fixed_allocator* create_allocator(std::size_t size) noexcept;

        /**
         * \brief Clearing of the sm_allocator instance, method deallocate all inner allocators
         * is used only for testing, calling this method in production code cause assertion failure
         */
        void clear();

        /**
         * \brief initializes all necessary fixed allocators, this method can be called only if memory saving mode is disabled,
         * otherwise the application will exit with assert. Early initialization saves allocation time, but requires a little more memory
         */
        void initialize_allocators();

        small_object_allocator();
        ~small_object_allocator();

        allocator_list m_allocator_list;                         // sorted vector (ascending), contains all available allocators
        std::size_t m_chunk_size{ config::ChunkSize };           // the number of blocks that one chunk contains
        std::size_t m_max_object_size{ config::MaxObjectSize };  // the max size of object, that is considered small

        friend class sm_allocator_reset;
    };

}
