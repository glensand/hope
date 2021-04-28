/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/components/common.h"
#include "hope/typelist/type_list_detail.h"

#include <type_traits>
#include <algorithm>
#include <array>

namespace hope {

    template <typename... Types>
    class type_list final {

    };

    template <typename... Ts>
    constexpr size_t size(type_list<Ts...>) {
        return sizeof...(Ts);
    }

    template<typename T, typename... Ts>
    constexpr type_holder<T> front(type_list<T, Ts...>) {
        return {};
    }

    template<typename T, typename... Ts>
    constexpr type_list<Ts...> pop_front(type_list<T, Ts...>) {
        return {};
    }

    template <typename T, typename... Ts>
    constexpr type_list<T, Ts...> push_front(type_list<Ts...>, type_holder<T>) {
        return {};
    }

    template <typename... Ts1, typename... Ts2>
    constexpr type_list<Ts1..., Ts2...> concat(type_list<Ts1...>, type_list<Ts2...>) {
        return {};
    }
    template <typename T, typename... Ts>
    constexpr type_holder<T> back(type_list<Ts..., T>) {
        return {};
    }

    template <typename T, typename... Ts>
    constexpr bool contains(type_list<Ts...>) {
        return (... || std::is_same_v<T, Ts>);
    }

    template <typename T, typename... Ts>
    constexpr bool contains14(type_list<Ts...>) {
        constexpr bool bs[] = { std::is_same_v<T, Ts>... };
        bool result = false;
        for (bool b : bs)
            result |= b;

        return result;
    }

    template <typename T, typename... Ts>
    constexpr std::size_t find(type_list<Ts...> list) {
        constexpr bool bs[] = { std::is_same_v<T, Ts>... };
        constexpr size_t list_size = size(list);

        for (size_t i = 0; i < list_size; ++i) {
            if (bs[i])
                return i;
        }
        return list_size;
    }

    template <typename T, std::size_t I, typename... Ts>
    constexpr std::size_t find_nth_of(type_list<Ts...> list) {
        constexpr bool bs[] = { std::is_same_v<T, Ts>... };
        constexpr size_t list_size = size(list);
        std::size_t index{ 0 };
        for (size_t i = 0; i < list_size; ++i) {
            if (bs[i]) {
                if (index == I)
                    return i;
                ++index;
            }
        }
        return list_size;
    }

    template <typename T, typename... Ts>
    constexpr std::size_t find_cxx20(type_list<Ts...>) {
        constexpr bool bs[] = { std::is_same_v<T, Ts>... };
        return std::find(bs, true) - bs;
    }

    template <template <typename...> typename F, typename... Ts>
    constexpr size_t find_if(type_list<Ts...>) {
        constexpr bool bs[] = { F<Ts>::value... };
        return std::find(bs, true) - bs;
    }

    template <typename F, typename... Ts>
    constexpr size_t find_if(type_list<Ts...> list, F f) {
        bool bs[] = { f(type_holder<Ts>{})... };
        /*return std::find(bs, true) - bs;*/ // available since cxx20
        constexpr size_t list_size = size(list);

        for (size_t i = 0; i < list_size; ++i) {
            if (bs[i])
                return i;
        }

        return list_size;
    }

    template <template <typename...> typename F, typename... Ts>
    constexpr bool all_of(type_list<Ts...>) {
        return (... && F<Ts>::value);
    }

    template <template <typename...> typename F, typename... Ts>
    constexpr bool any_of(type_list<Ts...>) {
        return (... || F<Ts>::value);
    }

    template <template <typename...> typename F, typename... Ts>
    constexpr bool none_of(type_list<Ts...> list) {
        return !any_of<F>(list);
    }

    template<size_t I, typename... Ts>
    constexpr auto get_nth_type(type_list<Ts...>) {
        return type_holder<decltype(
            detail::get<std::make_index_sequence<I>>::extractor(static_cast<Ts*>(nullptr)...))>{};
    }

    template<size_t I, typename... Ts>
    using NthType = typename decltype(get_nth_type<I>(type_list<Ts...>{}))::Type;

    template <typename F, typename...Ts, std::size_t... Is>
    constexpr void for_each_impl(type_list<Ts...> list, F&& f, std::index_sequence<Is...>) {
        (f(get_nth_type<Is>(list)), ...);
    }

    template <typename... Ts, typename F>
    constexpr void for_each(type_list<Ts...> list, F&& f) {
        for_each_impl(list, std::forward<F>(f),
            std::make_index_sequence<size(list)>()
        );
    }

    template <typename T, typename... Ts>
    constexpr size_t index_of(type_list<Ts...>) {
        constexpr bool bs[] = { std::is_same_v<T, Ts>... };
        return std::count_if(bs, true);
    }

    template <typename... Ts>
    constexpr size_t largest_type_index(type_list<Ts...>) {
        constexpr std::array<std::size_t, sizeof...(Ts)> sizes{ sizeof(Ts)... };
        const auto it = std::max_element(std::begin(sizes), std::end(sizes));
        return std::distance(std::begin(sizes), it);
    }

    template <typename... Ts>
    constexpr auto largest_type(type_list<Ts...> list) {
        constexpr auto index = largest_type_index(list);
        return get_nth_type<index>(list);
    }

    template <typename... Ts>
    constexpr type_list<Ts...> make_type_list(type_holder<Ts>&&...) {
        return {};
    }
}