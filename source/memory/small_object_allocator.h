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

#include "fixed_allocator.h"

namespace hope::memory {

    /**
     * \brief 
     */
    class small_object_allocator {
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
         * \brief singleton instance initialization
         * \param chunk_size the number of blocks that one chunk contains (in the range from 0 to 256); obviously
         * \param max_object_size the max size of object, that is considered small
         */
        void initialize(std::size_t chunk_size, std::size_t max_object_size) noexcept;

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

        small_object_allocator() = default;
        std::vector<fixed_allocator> m_allocator_list;  // sorted vector (ascending), contains all available allocators
        bool m_is_initialized{ false };                 // if singleton was initialized (has proper m_chunk_size and m_max_object_size)
        std::size_t m_chunk_size{ 0 };                  // the number of blocks that one chunk contains
        std::size_t m_max_object_size{ 0 };             // the max size of object, that is considered small
    };

}
