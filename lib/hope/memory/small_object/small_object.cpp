/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "hope/memory/small_object/small_object.h"
#include "hope/memory/small_object/small_object_allocator.h"
#include "hope/concurrency/spin_lock.h"
#include <mutex>

#ifdef MULTITHREADING
#   if THREADING_POLICY == SPINLOCK
#       define LOCK const std::lock_guard lock(SpinLock);
#   else
#       define LOCK const std::lock_guard lock(Mutex);
#   endif
#else 
#   define LOCK   
#endif

namespace
{
std::mutex  Mutex;
hope::concurrency::spin_lock SpinLock;
}

namespace hope::memory {
    void* small_object::operator new(std::size_t size) {
        LOCK
        return small_object_allocator::instance().allocate(size);
    }

    void small_object::operator delete(void* ptr, std::size_t size) {
        LOCK
        small_object_allocator::instance().deallocate(ptr, size);
    }
}
