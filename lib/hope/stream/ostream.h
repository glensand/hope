/* Copyright (C) 2021 - 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/foundation.h"
#include <cstdlib>

namespace hope{

    class ostream{
    public:
        ostream() = default;
        virtual ~ostream() = default;

        DECLARE_NON_COPYABLE(ostream);
        DECLARE_EXPLICIT_DEFAULT_MOVABLE(ostream);

        virtual bool is_open() const noexcept = 0;

        virtual void write(const void* data, std::size_t size) = 0;

        virtual void flush() = 0;

        template<typename T>
        void write(const T& value){
            write(&value, sizeof value);
        }
    };

}
