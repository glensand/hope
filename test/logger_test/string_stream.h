/* Copyright (C) 2021 - 2023 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "hope/stream/ostream.h"
#include <sstream>

namespace hope::log::testing {

    class string_stream final : public ostream {
    public:
        virtual ~string_stream() override = default;

        bool is_open() const noexcept override {
            return true;
        }

        void write(const void *data, std::size_t size) override {
            stream_impl.write((char*)data, size);
        }

        void flush() override {
            // nop
        }

        std::stringstream stream_impl;
    };

}
