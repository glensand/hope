//------------------------------------------------------------------------------
// IsSame.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 24.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "TrueType.h"

template <typename T1, typename T2>
struct IsSame : FalseType
{

};

template <typename T>
struct IsSame<T, T> : TrueType
{

};

template <typename T1, typename T2>
using IsSameT = typename IsSame<T1, T2>::Type;

template <typename T1, typename T2>
constexpr auto IsSameV = IsSame<T1, T2>::Value;
