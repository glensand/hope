//------------------------------------------------------------------------------
// IsDefaultConstructable.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 24.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "TrueType.h"

namespace Detail
{
	template <typename T>
	struct _IsDefaultConstructable
	{
	private:
		template <typename U, typename = decltype(U())>
		constexpr static TrueType TestConstructor(void*);
	
		template <typename>
		constexpr static FalseType TestConstructor(...);
	public:
		using Type = decltype(TestConstructor<T>(nullptr));
	};
}

template <typename T>
struct _IsDefaultConstructable : Detail::_IsDefaultConstructable<T>::Type
{
	
};

template <typename T>
constexpr auto IsDefaultConstructable = _IsDefaultConstructable<T>::Value;