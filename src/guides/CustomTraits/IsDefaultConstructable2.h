//------------------------------------------------------------------------------
// IsDefaultConstructable2.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 24.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "TrueType.h"
#include "IsSame.h"

namespace Detail
{
	template <typename ...> using Void = void;
	
	template <typename ...>
	struct IsDefaultConstructable2 : FalseType
	{

	};

	template <typename T, typename = Void<decltype(T())>>
	struct IsDefaultConstructable2 : TrueType
	{

	};
	
}

template <typename T>
constexpr auto IsDefaultConstructable = Detail::IsDefaultConstructable2<T>::Value;