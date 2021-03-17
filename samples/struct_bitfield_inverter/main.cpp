/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "tuple/generated.h"
#include "components/bit_utils.h"

#include <cassert>
#include <utility>

struct bit_struct_3 final {
	unsigned _0 : 3, _1 : 5;
	unsigned _2 : 7, _3 : 1;

	void init() {
		_0 = _1 = _2 = _3 = ~0;
	}
	// { ~0 }; // since cxx20
};

namespace {

	template <typename T>
	void invert_bit_endien(T& field, size_t count) {

	}

	template <typename S, typename... Ts, std::size_t... Is>
	void invert_structure_fields_endian_impl(hope::flat_tuple<Ts...>& tuple, std::index_sequence<Is...>) {
		S struct_to_count_field{ };
		struct_to_count_field.init();
		auto tuple_to_count_field = hope::tuple_from_struct(struct_to_count_field);
		(invert_bit_endien(tuple.template get<Is>(), hope::bit_utils::bit_count(tuple_to_count_field.template get<Is>())), ...);

	}

	template <typename S>
	void invert_struct_fields_endian(S& st) {
		auto tuple = hope::tuple_from_struct(st);
		invert_structure_fields_endian_impl<S>(
			tuple, std::make_index_sequence<tuple.get_size()>{});
	}
}

int main() {

	bit_struct_3 st{ };
	st._1 = 0;
	invert_struct_fields_endian(st);

	return 0;
} 