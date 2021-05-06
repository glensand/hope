#include "hope/serialization/package.h"

namespace hope::serialization {

void package::write(const void* data, std::size_t count) {
    const auto* ptr = static_cast<const uint8_t*>(data);
    buffer.insert(std::end(buffer), ptr, ptr + count);
}

void package::read(void* data, std::size_t count) {
    auto* ptr = static_cast<uint8_t*>(data);
    if(count == 1) // small optimization 
        *ptr = buffer[buffer_pos];
    else 
        std::copy_n(std::begin(buffer) + buffer_pos, count, ptr);
    
    buffer_pos += count;
}

}
