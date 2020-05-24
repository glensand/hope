//------------------------------------------------------------------------------
// TrueType.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 24.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

template <bool Val>
struct BoolConstant
{
	using Type = BoolConstant<Val>;
	static constexpr bool Value = Val;
};

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;