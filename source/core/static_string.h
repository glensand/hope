/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <algorithm>

namespace hope{

    template <std::size_t N>
    struct static_string final {

        constexpr static_string(const char(&str)[N]) {
            std::copy_n(str, N, value);
        }

        char value[N];
    };

}
