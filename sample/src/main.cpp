/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include <iostream>
#include "memory/objects_aligned_to4.h"
#include "typelist/type_list.h"
#include "memory/small_object.h"
#include "memory/sm_objects.h"
#include "memory/small_object_allocator.h"

#include <chrono>
#include <iostream>

namespace hope::memory::testing {

	template <typename T>
	bool fill_vector_impl(sm_list_t& vec, std::size_t size) {
		for (std::size_t i{ 0 }; i < size; ++i)
			vec.push_back(new T{ });
		return true;
	}

    template<template<typename Inner> typename T, std::size_t... Is, typename... Ts>
	void fill_vector_seq(std::index_sequence<Is...>, type_list<Ts...>list, sm_list_t& vec, std::size_t size) {
		// so, my msvc cant use fold expressions properly, thus i have to do such shitty trick with curly braces
		bool br[] = { fill_vector_impl<T<typename decltype(get_nth_type<Is>(list))::Type>>(vec, size)... };
	}

	template <template<typename Inner> typename T>
	auto fill_and_clock(std::size_t count) {
		sm_list_t sm_list;
		constexpr auto test_type_list = RegisteredTypesAscendingAlignedTo4{};
		constexpr auto sequence = std::make_index_sequence<size(test_type_list)>();
		const auto start_time = std::chrono::steady_clock::now();
		fill_vector_seq<T>(sequence, test_type_list, sm_list, count);
		const auto end_time = std::chrono::steady_clock::now();
		const auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
		std::cout << elapsed_ns.count() << std::endl;
		return elapsed_ns;
	}
}

int main()
{
	hope::memory::testing::fill_and_clock<simple_sm_object>(250);
	hope::memory::sm_allocator_reset::apply();
	hope::memory::testing::fill_and_clock<sm_object_std>(250);
	return 0;
} 