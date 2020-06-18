//------------------------------------------------------------------------------
// IsInvokable.h

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
	template <typename F, typename... Args,
		typename = decltype(DeclVal<F>()(DeclVal<Args&&>()...))>
	TrueType IsValidImpl(void*);

	template <typename F, typename... Args>
	FalseType IsValidImpl(...);

	constexpr
	auto IsValid = [](auto f)
	{
		return [](auto&&... args)
		{
			return
				decltype(IsValidImpl<decltype(f),
				decltype(args) &&...>(nullptr)){};
		};
	};

	template <typename T>
	struct Type
	{
		using Type = T;
	};

	template <typename T>
	constexpr auto type = Type<T>{};

	template <typename T>
	T Value(Type<T>);
}

constexpr auto IsDefaultConstructable
= Detail::IsValid([](auto x)->decltype((void)decltype(Detail::Value(x))()) {});
