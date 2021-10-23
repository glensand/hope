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
#include "hope/components/detector.h"

#include <functional>

/*! \defgroup <reflection> Static reflection
    @{
*/

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

        template<typename TValue>
        using equal_operation_t = decltype(std::declval<TValue>() == std::declval<TValue>());
    }

    template <typename... Ts>
    constexpr bool operator==(const flat_tuple<Ts...>& left, const flat_tuple<Ts...>& right) {
        using tuple_t = flat_tuple<Ts...>;
        return detail::is_equal(left, right, std::make_index_sequence<tuple_t::get_size()>());
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

    template<typename TStruct>
    bool compare(const TStruct& lhs, const TStruct& rhs,
                float float_eps = std::numeric_limits<float>::epsilon(), 
                double double_eps = std::numeric_limits<double>::epsilon()) noexcept {
        
        static_assert(std::is_class_v<TStruct>, 
            "hope::compare: Inbuilt type has been provided, only structures and classes are allowed");

        auto&& left = tuple_from_struct(lhs, hope::field_policy::reference{});
        auto&& right = tuple_from_struct(rhs, hope::field_policy::reference{});
        auto equal = true;
        hope::for_each( // NOTE: do not remove hope::, 'cause utility like foreach is in conflict with stl for_each.
            left, right, 
            [&](auto&& l, auto&& r){
                if (!equal) return;
                using value_t = std::decay_t<decltype(l)>;
                // msvc sucks and tries to compile else branch in constexpr statement (is is legal?)
                constexpr bool has_equal_operator = is_detected_v<detail::equal_operation_t, value_t>; 
                if constexpr (std::is_same_v<value_t, float>){
                    equal = std::abs(l - r) < float_eps;
                } else if constexpr (std::is_floating_point_v<value_t>){
                    equal = std::abs(l - r) < (value_t)double_eps;
                } else if constexpr (has_equal_operator){
                    equal = l == r;
                } else if constexpr (!has_equal_operator){
                    equal = compare(l, r, float_eps, double_eps);
                }
            }
        );
        return equal;
    }
}

template <typename TStruct>
constexpr std::enable_if_t<hope::is_user_defined_type_v<TStruct>, bool>
operator==(const TStruct& left, const TStruct& right) {
    return hope::tuple_from_struct(left, hope::field_policy::reference{}) == hope::tuple_from_struct(right, hope::field_policy::reference{});
}

/*! @} */