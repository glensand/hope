/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/typelist/type_list.h"

namespace hope {
    constexpr static type_list
        <bool,
        unsigned char, char,
        unsigned short, short,
        unsigned int, int,
        unsigned long, long,
        unsigned long long, long long,
        float, double, long double,
        void*, std::nullptr_t, const void*>
        IntegralTypes { };

    constexpr static std::size_t IntegralTypesCount{ size(IntegralTypes) };
}

