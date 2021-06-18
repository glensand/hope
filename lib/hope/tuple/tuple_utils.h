/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/tuple/flat_tuple.h"
#include "hope/tuple/tuple_from_struct.h"
#include "hope/components/user_defined_types.h"
#include <functional>

namespace hope {

    namespace detail {

        template<typename T>
        constexpr std::enable_if_t<!is_user_defined_type_v<T>, bool>
            is_equal(const T& left, const T& right);

        template<typename... Ts, std::size_t... Is>
        constexpr bool is_equal(const flat_tuple<Ts...>& left, const flat_tuple<Ts...>& right, std::index_sequence<Is...>);

        template<typename T>
        constexpr std::enable_if_t<is_user_defined_type_v<T>, bool>
            is_equal(const T& left, const T& right);

        template<typename T>
        constexpr std::enable_if_t<!is_user_defined_type_v<T>, bool>
            is_equal(const T& left, const T& right) {
            return left == right;
        }

        template<typename... Ts, std::size_t... Is>
        constexpr bool is_equal(const flat_tuple<Ts...>& left, const flat_tuple<Ts...>& right, std::index_sequence<Is...>) {
            return (is_equal(left.template get<Is>(), right.template get<Is>()) && ...);
        }

        template<typename T>
        constexpr std::enable_if_t<is_user_defined_type_v<T>, bool>
            is_equal(const T& left, const T& right) {
            return tuple_from_struct(left, field_policy::reference{}) == tuple_from_struct(right, field_policy::reference{});
        }

        template<typename T>
        constexpr std::size_t hash(const T& val) {
            return std::hash<T>{}(val);
        }

        template<typename... Ts, std::size_t... Is>
        constexpr std::size_t hash(const flat_tuple<Ts...>& tuple, std::index_sequence<Is...>) {
            constexpr auto tuple_size = size(type_list<Ts...>{});
            return (std::size_t(hash(tuple. template get<Is>()) / tuple_size) + ...);
        }
    }

    template <typename... Ts>
    constexpr bool operator==(const flat_tuple<Ts...>& left, const flat_tuple<Ts...>& right) {
        return detail::is_equal(left, right, std::make_index_sequence<left.get_size()>());
    }

    template <typename... Ts>
    constexpr bool operator!=(const flat_tuple<Ts...>& left, const flat_tuple<Ts...>& right) {
        return !(left == right);
    }

    template <typename... Ts>
    constexpr bool operator<(const flat_tuple<Ts...>& left, const flat_tuple<Ts...>& right) {
        return detail::is_equal(left, right, std::make_index_sequence<flat_tuple<Ts...>::size>());
    }

    template <typename... Ts>
    struct hash final {
        constexpr std::size_t operator()(const flat_tuple<Ts...>& tuple) {
            return detail::hash(tuple, std::make_index_sequence < size(type_list<Ts...>{})() > ());
        }
    };
}

template <typename TStruct>
constexpr std::enable_if_t<hope::is_user_defined_type_v<TStruct>, bool>
operator==(const TStruct& left, const TStruct& right) {
    return hope::tuple_from_struct(left, hope::field_policy::reference{}) == hope::tuple_from_struct(right, hope::field_policy::reference{});
}