/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 *
 * The main idea of the utilities which are located here is honestly borrowed by Björn Fahller report form cpp Piter 2019 conference
 */

#pragma once

#include <type_traits>
#include "hope/tuple/flat_tuple.h"

namespace hope {

    template <typename... Ts>
    class all final {
    public:
        template<typename... Vs>
        constexpr explicit all(Vs&&... args) noexcept
            : tuple(std::forward<Vs>(args)...) { }

        template <typename T>
        constexpr bool operator==(const T& rhs) const noexcept {
            bool res = true;
            for_each(tuple, [&](const auto& lhs) {
                res &= lhs == rhs;
            });
            return res;
        }

        template <typename T>
        friend constexpr bool operator==(const T& lhs, const all& rhs) noexcept {
            return rhs == lhs;
        }

        template <typename T>
        constexpr bool operator!=(const T& rhs) const noexcept {
            return !((*this) == rhs);
        }

        template <typename T>
        friend constexpr bool operator!=(const T& lhs, const all& rhs) noexcept {
            return rhs != lhs;
        }

    private:
        flat_tuple<std::decay_t<Ts>...> tuple;
    };

    template <typename... Ts>
    all(Ts...)->all<Ts...>;

    template <typename... Ts>
    class any final {
    public:
        template<typename... Vs>
        constexpr explicit any(Vs&&... args) noexcept
            : tuple(std::forward<Vs>(args)...) { }

        template <typename T>
        constexpr bool operator==(const T& rhs) const noexcept {
            bool res = false;
            for_each(tuple, [&](const auto& lhs) {
                res |= lhs == rhs;
            });
            return res;
        }

        template <typename T>
        friend constexpr bool operator==(const T& lhs, const any& rhs) noexcept {
            return rhs == lhs;
        }

        template <typename T>
        constexpr bool operator!=(const T& rhs) const noexcept {
            return !(tuple == rhs);
        }

        template <typename T>
        friend constexpr bool operator!=(const T& lhs, const any& rhs) noexcept {
            return rhs != lhs;
        }

    private:
        flat_tuple<std::decay_t<Ts>...> tuple;
    };

    template <typename... Ts>
    any(Ts...)->any<Ts...>;
}