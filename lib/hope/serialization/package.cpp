#include "hope/serialization/package.h"

#include <algorithm>
#include <stdexcept>

namespace hope::serialization {

void package::write(const void* data, std::size_t count) {
    if (buffer.size() <= write_pos + count)
        buffer.resize(write_pos + count + 1);

    const auto* ptr = static_cast<const unsigned char*>(data);
    std::copy_n(ptr, count, buffer.data() + write_pos);
    write_pos += count;
}

void package::read(void* data, std::size_t count) {
    if (read_pos >= write_pos)
        throw std::runtime_error("package::read: Not enough data in  the buffer");

    if(auto* ptr = static_cast<unsigned char*>(data); count == 1) // small optimization 
        *ptr = buffer[read_pos];
    else 
        std::copy_n(std::begin(buffer) + read_pos, count, ptr);
    
    read_pos += count;
}

std::size_t package::seek(std::size_t position) {
    const auto prev_buffer_pos = write_pos;
    if(position > write_pos)
        buffer.resize(position);
    write_pos = position;

    return prev_buffer_pos;
}

std::size_t package::reserve(std::size_t count) {
    return seek(count + write_pos);
}

void package::clear() {
    buffer.clear();
    write_pos = 0;
    read_pos = 0;
}

}
