/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <functional>

#include "memory/small_object.h"
#include "typelist/type_list.h"

template <typename T>
struct sm_object : hope::memory::small_object {
    T val;
};

template <typename T>
struct sm_object_std {
    virtual ~sm_object_std() = default;
};

namespace hope::memory {

	class reset_sm_allocator {
	public:
		static void apply() {
			small_object_allocator::instance().clear();
			small_object_allocator::allocator_list bleach;
			std::swap(small_object_allocator::instance().m_allocator_list, bleach);
		}
	};

	namespace testing {
		using sm_list_t = std::vector<small_object*>;
		using sm_list_modifier_t = std::function<void(sm_list_t& list)>;

	    template<std::size_t... Is, typename... Ts>
		void fill_vector(std::index_sequence<Is...> seq, type_list<Ts...> list, sm_list_t& vec) {
			small_object* br[] = { new sm_object<typename decltype(get_nth_type<Is>(list))::Type>... };
			vec.insert(std::end(vec), std::begin(br), std::end(br));
		}

		template <typename... Ts>
		void alloc_dealloc(type_list<Ts...> list, std::size_t objects_count, const sm_list_modifier_t& modifier) {
			constexpr auto sequence = std::make_index_sequence < size(list) >();
			sm_list_t sm_list;
	        for (std::size_t i = 0; i < objects_count; ++i)
				fill_vector(sequence, list, sm_list);
	        modifier(sm_list);
	        for (const auto object : sm_list)
				delete object;
	        sm_list = sm_list_t{};
			reset_sm_allocator::apply();
		}
	}
}