/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <array>
#include <iostream>

namespace hope{

    template <std::size_t N>
    using static_string = std::array<const char, N>;

    namespace impl {

        template <std::size_t N, std::size_t... Is>
        constexpr auto make_static_string(const char(&str)[N], std::index_sequence<Is...>) noexcept {
            return static_string<N + 1>{ str[Is]..., '\0' };
        }

        template <std::size_t S1, std::size_t S2,
        std::size_t... Is1, std::size_t... Is2>
        constexpr auto concat(const static_string<S1>& str1, const static_string<S2>& str2,
            std::index_sequence<Is1...>, std::index_sequence<Is2...>){
            return hope::static_string<S1 + S2 + 1>{ str1[Is1]..., str2[Is2]..., '\0'};
        }

    }

    template<std::size_t N>
    constexpr auto make_static_string(const char (&str)[N]) noexcept {
        return impl::make_static_string(str, std::make_index_sequence<N>());
    }

    template <std::size_t S1, std::size_t S2>
    constexpr auto concat(const static_string<S1>& str1, const static_string<S2>& str2) noexcept {
        return impl::concat(str1, str2, std::make_index_sequence<S1 - 2>(), std::make_index_sequence<S2 - 2>());
    }
}

template <std::size_t N>
std::ostream& operator<<(std::ostream& stream, const hope::static_string<N>& str) {
    stream << str.data();
    return stream;
}
