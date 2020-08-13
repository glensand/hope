//------------------------------------------------------------------------------
// type_list.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 30.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include <type_traits>
#include <algorithm>
#include <array>

namespace hope {

    template <typename T>
    struct type_holder {
        using Type = T;
    };

    template <typename... Types>
    class type_list {

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

    template <typename T, typename... Ts>
    constexpr std::size_t Find20(type_list<Ts...>) {
        constexpr bool bs[] = { std::is_same_v<T, Ts>... };
        return std::find(bs, true) - bs;
    }

    template <template <typename...> typename F, typename... Ts>
    constexpr size_t find_if(type_list<Ts...>) {
        constexpr bool bs[] = { F<Ts>::value... };
        return std::find(bs, true) - bs;
    }

    // TODO:: maybe it is good idea to use value based approach every were
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

    namespace detail {
        template <typename Is>
        struct get;

        template <size_t... Is>
        struct get<std::index_sequence<Is...>> {
            template <typename T>
            static constexpr T extractor(decltype(Is, static_cast<void*>(0))..., T*, ...);
        };
    }

    template<size_t I, typename... Ts>
    constexpr auto get_nth_type(type_list<Ts...>) {
        return type_holder<decltype(
            detail::get<std::make_index_sequence<I>>::extractor((Ts*)(0)...))>{};
    }

    template<size_t I, typename... Ts>
    using NthType = typename decltype(get_nth_type<I>(type_list<Ts...>{}))::Type;

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