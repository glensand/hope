//------------------------------------------------------------------------------
// integraltypes.h
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 15.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "typelist/type_list.h"

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

