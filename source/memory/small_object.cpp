/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "small_object.h"
#include "small_object_allocator.h"

namespace hope::memory {
    void* small_object::operator new(std::size_t size) {
        return small_object_allocator::instance().allocate(size);
    }

    void small_object::operator delete(void* ptr, std::size_t size) {
        small_object_allocator::instance().deallocate(ptr, size);
    }
}
