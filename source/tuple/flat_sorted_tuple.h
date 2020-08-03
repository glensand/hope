//------------------------------------------------------------------------------
// FlatSortedTuple.h
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 19.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "typelist/type_list.h"
#include "typelist/typelistsort.h"

namespace hope {
    namespace detail {

        template<typename... Ts, typename... Vs>
        constexpr auto make_flat_tuple_from_type_list(type_list<Ts...>, Vs&&... args) {
            return flat_tuple<Ts...>();
        }
    }

    template<typename... Ts>
    constexpr auto make_sorted_tuple(Ts... args) {
        constexpr auto typeList = sort(type_list<Ts...>{});
        return detail::make_flat_tuple_from_type_list(typeList, std::forward<Ts>(args)...);
    }
}
