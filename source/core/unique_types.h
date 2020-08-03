//------------------------------------------------------------------------------
// UniqueTypes.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 4.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include <type_traits>

template <typename ... Ts>
struct unique_types;

template <typename T1, typename T2, typename ... Ts>
struct unique_types<T1, T2, Ts ...>
    : unique_types<T1, T2>, unique_types<T1, Ts ...>, unique_types<T2, Ts ...> {};

template <typename T1, typename T2>
struct unique_types<T1, T2> {
    static_assert(!std::is_same_v<T1, T2>, "Types must be unique.");
};