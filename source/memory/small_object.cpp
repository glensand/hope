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
