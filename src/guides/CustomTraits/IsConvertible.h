//------------------------------------------------------------------------------
// IsConvertible.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 24.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "TrueType.h"
#include "Declval.h"

namespace Detail
{
	
	template <typename From, typename To>
	struct IsConvertible
	{
	private:
		constexpr static void Try(To);

		template <typename F, typename, typename = decltype(Try(DeclVal<F>()))>
		constexpr static TrueType Test(void*);

		template <typename, typename>
		constexpr static FalseType Test(...);

	public:
		using Type = decltype(Test<From>(nullptr));
	};

}

template <typename From, typename To>
struct _IsConvertible : Detail::IsConvertible<From, To>::Type
{

};

template <typename From, typename To>
using IsConvertibleT = typename _IsConvertible<From, To>::Type;

template <typename From, typename To>
constexpr auto IsConvertibleV = _IsConvertible<From, To>::Value;