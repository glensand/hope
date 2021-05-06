/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <vector>

namespace hope::serialization {

    class package final {
        using buffer_t = std::vector<uint8_t>;
    public:
        package() = default;

        template <typename T>
        void write(T val) {
            write(&val, sizeof val);
        }

        void write(const void* data, std::size_t count);

        template <typename T>
        T read() {
            T val;
            read(&val, sizeof val);
            return val;
        }

        void read(void* data, std::size_t count);

        [[nodiscard]] const buffer_t& get_buffer() const noexcept { return buffer; }

        void clear() { buffer.clear(); }
    private:
        buffer_t buffer;
        std::size_t buffer_pos{ 0u };
    };

}
