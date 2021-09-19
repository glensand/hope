#include "stack_buffer.h"

namespace hope{

    void stack_buffer::put(const void *data, std::size_t size) noexcept {
        auto&& free_space = BufferSize - bytes_written;
        auto write_to_stack_buffer = free_space > size ? size : free_space;
        auto* begin = (char*)data;
        std::copy(begin, begin + write_to_stack_buffer, buffer.data() + bytes_written);
        bytes_written += write_to_stack_buffer;
        // if some amount of bytes does not fit to the stack buffer, copy it to the heap
        for(std::size_t i{ write_to_stack_buffer }; i < size; ++i){
            additional_buffer.emplace_back(begin[i]);
        }
    }

}
