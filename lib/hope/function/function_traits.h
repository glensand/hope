/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/typelist/type_list.h"

namespace hope {

    template <typename T>
    struct function_traits final : function_traits<decltype(&T::operator())> {};

    template <typename TClass, typename TReturn, typename... Ts>
    struct function_traits<TReturn(TClass::*)(Ts...) const> {
        using result_t = TReturn;
        static constexpr type_list<Ts...> arg_types;
        static constexpr auto arity = sizeof...(Ts);
    };

    template <typename TClass, typename TReturn, typename... Ts>
    struct function_traits<TReturn(TClass::*)(Ts...)> {
        using result_t = TReturn;
        static constexpr type_list<Ts...> arg_types;
        static constexpr auto arity = sizeof...(Ts);
    };

}
