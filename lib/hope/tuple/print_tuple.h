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
#include <functional>
#include <iostream>

namespace hope{

    namespace detail {

        template<typename... Ts, std::size_t... VIs>
        void print_impl(std::ostream& stream, const flat_tuple<Ts...>& tuple, std::index_sequence<VIs...>) {
            stream << "{ ";
            ((stream << (VIs == 0 ? "" : ", ") << tuple.template get<VIs>()), ...);
            stream << " };";
        }
    
    }

    template<typename... Ts>
    constexpr std::ostream& operator<< (std::ostream& stream, const flat_tuple<Ts...>& tuple) {
        print_impl(stream, tuple, std::make_index_sequence<size(type_list<Ts...>{})>());
        return stream;
    }
}


template<typename T>
constexpr std::ostream& operator<< (std::ostream& stream, const T& object) {
    auto&& tuple = hope::ref_tuple_from_struct(object);
    stream << tuple;
    return stream;
}