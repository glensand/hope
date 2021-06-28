/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/tuple/flat_tuple.h"
#include "hope/tuple/detect_fields_count.h"
#include "hope/typelist/type_list.h"
#include "hope/components/loophole.h"

namespace hope {
    namespace detail{

        template <typename T>
        constexpr T construct_helper() noexcept {
            return {};
        }

        template <typename Struct, std::size_t I>
        struct any_convertible_s {

            using struct_t = std::decay_t<Struct>;

            template <typename Row>
            constexpr operator Row () const noexcept {
                using clean_t = std::decay_t<Row>;
                loophole::inject<I>(struct_t{}, Row{});
                return construct_helper<clean_t>();
            }
        };

        template<typename... Ts>
        constexpr auto tuple_from_type_list(type_list<Ts...>) {
            return flat_tuple<Ts...>{};
        }

        template <typename T, std::size_t... Is>
        constexpr auto extract_types(std::index_sequence<Is...> sequence) {
            constexpr T object{ any_convertible_s<T, Is>{}... };
            (void)object;
            return type_list<decltype(loophole::get<Is>(std::declval<T>()))...>{};
        }
    }

    template <typename T>
    constexpr auto make_tuple() {
        constexpr auto fields_count = detect_fields_count<T>();
        constexpr auto types = detail::extract_types<T>(std::make_index_sequence<fields_count>());
        auto tuple = detail::tuple_from_type_list(types);
        return tuple;
    }

    template <typename T>
    auto tuple_from_struct_unsafe(const T& object) {
        constexpr auto tuple = make_tuple<T>();
        (T&)tuple = object;
        return tuple;
    }
}
