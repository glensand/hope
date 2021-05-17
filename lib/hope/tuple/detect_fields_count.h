/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <type_traits>

namespace hope {
    namespace detail {

        template <std::size_t I>
        struct any_convertible {
            template <typename T>
            constexpr operator T& () const noexcept;
        };

        template <typename T, std::size_t... Is>
        constexpr auto is_constructable_n(const T&, std::index_sequence<Is...>)
            ->decltype(T{ any_convertible<Is>{}... }, bool()) {
            return true;
        }

        template <typename T, std::size_t... Is>
        constexpr auto is_constructable_n(const T&, ...) {
            return false;
        }

        template <typename T, std::size_t... Is>
        constexpr std::size_t detect_fields_count_impl(const T& object, std::index_sequence<Is...> sequence) {
            bool bs[] = { is_constructable_n(object, std::make_index_sequence<Is>{ })... };
            // since cxx20... msvc sucks
            //return std::distance(std::begin(bs), std::find(
            //    std::begin(bs), std::end(bs), false)
            //);

            for (std::size_t i{ 1 }; i < sequence.size(); ++i) {
                if (!bs[i]) return i - 1;
            }
            return 0;
        }
    }

    template <typename T, bool HasBitField>
    constexpr std::size_t detect_fields_count(const T& object, std::integral_constant<bool, HasBitField>) {
        constexpr unsigned bit_multiplier{ HasBitField ? 8 : 1 };
        constexpr auto size = sizeof(T) * bit_multiplier;
        return detail::detect_fields_count_impl(object, std::make_index_sequence<size + 2>());
    }
}
