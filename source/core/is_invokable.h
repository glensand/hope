//------------------------------------------------------------------------------
// IsInvokable.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 24.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once
#include <type_traits>

namespace detail {

	template <typename F, typename... Args,
		typename = decltype(std::declval<F>()(std::declval<Args&&>()...))>
	std::true_type is_valid_impl(void*);

	template <typename F, typename... Args>
	std::false_type is_valid_impl(...);

	constexpr auto is_valid = [](auto f) {
		return [](auto&&... args) {
			return
				decltype(is_valid_impl<decltype(f),
				decltype(args) &&...>(nullptr)){};
		};
	};

	template <typename T>
	struct just_type {
		using Type = T;
	};

	template <typename T>
	constexpr auto type = just_type<T>{};

	template <typename T>
	T value(just_type<T>);
}

namespace hope {
	constexpr auto is_default_constructable
		= detail::is_valid([](auto x)->decltype((void)decltype(detail::value(x))()) {});
}

