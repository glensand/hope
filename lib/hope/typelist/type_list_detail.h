/* Copyright (C) 2021 - 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <utility>
#include <cstddef> // NOTE: do not remove dis header, it is required for successful g++ compilation (size_t definition) 

namespace hope::detail {

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
    namespace msvc_hack {
        template <typename... Types>
        class type_list_msvc final { };

        template<std::size_t I>
        struct size_t_matcher final { };

        template<typename T, typename... Ts>
        static constexpr auto extract(size_t_matcher<0>, type_list_msvc<T, Ts...>) {
            return (T*)nullptr;
        }

        template<std::size_t I, typename T, typename... Ts>
        static constexpr auto extract(size_t_matcher<I>, type_list_msvc<T, Ts...>) {
            return extract(size_t_matcher<I - 1>{}, type_list_msvc<Ts...>{});
        }
    }

    template<std::size_t I, typename... Ts>
    static constexpr auto* extract() {
        return msvc_hack::extract(msvc_hack::size_t_matcher<I>{}, msvc_hack::type_list_msvc<Ts...>{});
    }

#else

    template <typename Is>
    struct get;

    template <size_t... Is>
    struct get<std::index_sequence<Is...>> {
        template <typename T>
        static constexpr T extractor(decltype(Is, static_cast<void*>(0))..., T*, ...);
    };
#endif

}