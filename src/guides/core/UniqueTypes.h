//------------------------------------------------------------------------------
// UniqueTypes.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 4.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "IsSame.h"

template <typename ... Ts>
struct UniqueTypes;

template <typename T1, typename T2, typename ... Ts>
struct UniqueTypes<T1, T2, Ts ...>
    : UniqueTypes<T1, T2>, UniqueTypes<T1, Ts ...>, UniqueTypes<T2, Ts ...> {};

template <typename T1, typename T2>
struct UniqueTypes<T1, T2>
{
    static_assert(!IsSameV<T1, T2>, "Types must be unique.");
};