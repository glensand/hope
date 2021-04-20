/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

namespace hope::memory::config{

    /**
     * \brief Maximum size of object witch considered small, larger objects will be allocated using the standard new operator
     * It might make sense to increase this parameter. I use it by default as it was used by A. Alexandrescu in his book,
     * however this book was released in the early 2000s and the hardware capabilities have changed significantly since then.
     * I any case, this parameter must be configured for a specific application
     */
    constexpr size_t MaxObjectSize{ 64 };

    /**
     * \brief The number of blocks into which the memory chunk will be conditionally divided.
     * This value cannot be more than 2^8, since the block index in the chunk must fit into one byte.
     * At the moment, just such an implementation of a singly linked list is used. Maybe in the future,
     * I will revisit this point and make the parameter configurable
     */
    constexpr unsigned char ChunkSize{ 255 };

    /**
     * \brief Current alignment of the pointer. The parameter makes sense if the memory alignment on the heap is not disabled,
     * this allows you to find a suitable fixed allocator in O(1), instead of O(logN), where N is the number of fixed allocators
     * This parameter is also used if all fixed allocators are initialized in the small object allocator constructor
     */
    constexpr size_t PointerAlignment{ alignof(void*) };

    /**
     * \brief Indicates that the test mode is now activated, the test mode flag is required for some methods to work. For example,
     * deinitialization and reinitialization of the allocator of small objects.
     */
    constexpr bool TestMode{ true };

    /**
     * \brief Memory saving mode. If this flag is enabled, then fixed allocators will not be initialized along with the singleton.
     * In this case, at first the allocation of a new object will take additional time to find a suitable fixed allocator or to resize the vector
     * I recommend using this parameter if the size of the maximum object is large enough, few objects are allocated,
     * or the alignment of memory allocated on the heap is disabled.
     */
    constexpr bool MemoryReductionMode{ false };


    /**
     * \brief If multi threading mode is currently enabled. If this value is set to true, this mean if you try to create or delete an object
     * via new or delete, then a mutex would be locked.  do not recommend using this option if your application is running in single threaded mode,
     * or if all small objects will be created in the same thread. If you need multithreading support,
     * it is better to use multiple allocators with thread-local-storage inside.
     */
    constexpr bool MultiTreading{ true };

    /**
     * \brief Enumeration of available threading policies, there are no reason to use it, if MultiTreading mode configured as false
     */
    enum class EThreadingPolicy {
        Mutex = 0,
        SpinLock = 1,
    };

    /**
     * \brief Defines what kind of synchronization(block) will be use. If you threads rarely meets in critical section i recommend to use
     * spin lock, otherwise mutex.
     */
    constexpr EThreadingPolicy ThreadingPolicy{ EThreadingPolicy::SpinLock };
}

/**
 * \brief Enumeration of available threading policies, there are no reason to use it, if MultiTreading mode configured as false
 */
#define SPINLOCK 1
#define MUTEX 2

 /**
  * \brief Defines what kind of synchronization(block) will be use. If you threads rarely meets in critical section i recommend to use
  * spin lock, otherwise mutex.
  */
#define THREADING_POLICY SPINLOCK

/**
 * \brief If multi threading mode is currently enabled. If this value is set to true, this mean if you try to create or delete an object
 * via new or delete, then a mutex would be locked.  do not recommend using this option if your application is running in single threaded mode,
 * or if all small objects will be created in the same thread. If you need multithreading support,
 * it is better to use multiple allocators with thread-local-storage inside.
 */
#define MULTITHREADING 1
