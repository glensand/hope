/* Copyright (C) 2020 - 2022 Gleb Bezborodov - All Rights Reserved
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

/*! \defgroup <reflection> Static reflection
    @{
    \file
    \brief This file contains function which might be used to create tuple of specified types. Before tuple creation all
    the corresponding types sorts and reorders by predicate.
*/

namespace hope {
    namespace detail {

        template<typename... Ts, typename... Vs>
        constexpr auto make_flat_tuple_from_type_list(type_list<Ts...>, Vs&&... args) {
            return flat_tuple<Ts...>(std::forward<Vs>(args)...);
        }
    }

    /**
     * Function creates tuple of specified types with given values; Firstly types sorts by size (descending order).
     * Then values reorder is required.
     * \warning NOTE: This function is not implemented yet. And does not
     * @tparam Ts
     * @param args
     * @return
     */
    template<typename... Ts>
    constexpr auto make_sorted_tuple(Ts&&... args) {
        constexpr auto typeList = sort(type_list<Ts...>{});
        return detail::make_flat_tuple_from_type_list(typeList, std::forward<Ts>(args)...);
    }
}

/*! @} */