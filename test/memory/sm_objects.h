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

struct sm_object_uni {
	virtual ~sm_object_uni() = default;
};

template <typename T>
struct sm_object_std : sm_object_uni {
	T val;
};
template <typename T>
struct simple_sm_object final : hope::memory::small_object
                                , sm_object_uni {
    T val;
};

template <typename T>
struct complicated_sm_object final : hope::memory::small_object
                                    , sm_object_uni {
	T* val{ nullptr };
	complicated_sm_object() {
		val = new T;
	}

	~complicated_sm_object() {
		delete val;
	}
};

struct srting_sm_object final : hope::memory::small_object
                              , sm_object_uni {
	std::string val;
	srting_sm_object(std::string _val = "")
    : val(std::move(_val))
    {
	}

	~srting_sm_object() = default;
};

struct srting_sm_object2 final : hope::memory::small_object
                               , sm_object_uni {
    std::string val;
	int pos{ 0 };
	srting_sm_object2(std::string _val = "", int _pos = 0)
		: val(std::move(_val))
	    , pos(_pos)
	{
	}

	~srting_sm_object2() = default;
};

namespace hope::memory {

	class sm_allocator_reset {
	public:
		static void apply() {
			small_object_allocator::instance().clear();
			small_object_allocator::allocator_list bleach; // lol bleach
			std::swap(small_object_allocator::instance().m_allocator_list, bleach);
		}
	};

	namespace testing {
		using sm_list_t = std::vector<sm_object_uni*>;
		using sm_list_modifier_t = std::function<void(sm_list_t& list)>;
		using sm_list_validator_t = std::function<bool(const sm_list_t& list)>;

	    template<template<typename Inner> typename T, std::size_t... Is, typename... Ts>
		void fill_vector(std::index_sequence<Is...>, type_list<Ts...>list, sm_list_t& vec) {
			sm_object_uni* br[] = { new T<typename decltype(get_nth_type<Is>(list))::Type>... };
			vec.insert(std::end(vec), std::begin(br), std::end(br));
		}

		template <template<typename Inner> typename T, typename... Ts>
		void alloc_dealloc(type_list<Ts...> list, std::size_t objects_count, const sm_list_modifier_t& modifier) {
			constexpr auto sequence = std::make_index_sequence < size(list) >();
			sm_list_t sm_list;
	        for (std::size_t i = 0; i < objects_count; ++i)
				fill_vector<T>(sequence, list, sm_list);
	        modifier(sm_list);
	        for (const auto object : sm_list)
				delete object;
		}

		template <template<typename> typename T, typename... Ts>
		void apply_alloc_dealloc(type_list<Ts...> list, std::size_t objects_count) {
			alloc_dealloc<T>(list, objects_count, [](auto&&) {});
			alloc_dealloc<T>(list, objects_count,
				[](auto&& sm_list) {
					auto rng = std::default_random_engine{};
					std::shuffle(std::begin(sm_list), std::end(sm_list), rng);
				});
			alloc_dealloc<T>(list, objects_count,
				[](auto&& sm_list) {
					std::size_t i = 0;
					sm_list.erase(std::remove_if(std::begin(sm_list), std::end(sm_list),
						[&](auto&& sm)
						{
							if (i % 2 == 0)
								delete sm;
							++i;
							return i % 2;
						}), std::end(sm_list));
				});
		}
	}
}

