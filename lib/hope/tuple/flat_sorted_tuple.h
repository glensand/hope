/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/typelist/type_list.h"
#include "hope/typelist/typelistsort.h"
#include "hope/tuple/flat_tuple.h"

namespace hope {
    namespace detail {

        template<typename... Ts, typename... Vs>
        constexpr auto make_flat_tuple_from_type_list(type_list<Ts...>, Vs&&... args) {
            return flat_tuple<Ts...>(std::forward<Vs>(args)...);
        }
    }

    template<typename... Ts>
    constexpr auto make_sorted_tuple(Ts&&... args) {
        constexpr auto typeList = sort(type_list<Ts...>{});
        return detail::make_flat_tuple_from_type_list(typeList, std::forward<Ts>(args)...);
    }
}
