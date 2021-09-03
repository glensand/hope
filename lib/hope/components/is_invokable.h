/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

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

