/* Copyright (C) 2020 - 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/tuple/tuple_from_struct.h"
#include "hope/tuple/tuple_from_struct_unsafe.h"
#include "hope/tuple/tuple_policy.h"
#include "hope/components/user_defined_types.h"
#include "hope/components/detector.h"

/*! \defgroup <reflection> Static reflection
    @{
    \file
    \brief the file contains several functions for recursively calculating the number of fields within the structure.
    Only user-defined types are computed recursively, the classes of the standard library are considered a single whole
*/

namespace hope {

    /**
     * Implementation of the compile - time fields count computer. Is intended to use with c++ aggregates
     * @tparam TStructure Structure structure the number of fields to be calculated
     * @return Number of fields
     */
    template<typename TStructure>
    constexpr std::size_t compute_field_count_recursive_constexpr() {
        if constexpr (is_user_defined_type_v<TStructure>) {
            constexpr auto fields_count = detect_fields_count<TStructure>();
            constexpr auto types = detail::extract_types<TStructure>(std::make_index_sequence<fields_count>());
            std::size_t count{0};
            for_each(types, [&](auto field) {
                using type_t = typename decltype(field)::Type;
                count += compute_field_count_recursive_constexpr<type_t>();
            });
            return count;
        } else {
            return 1u;
        }
    }

    /**
     * Implementation of the run - time fields count computer; NOTE: each subfield will be instantiated,
     * be aware of usages of this function for heavy - weight objects.
    * @tparam TStructure Structure structure the number of fields to be calculated
     * @return Number of fields
     */
    template<typename TStructure>
    std::size_t compute_field_count_recursive() {
        if constexpr(is_user_defined_type_v<TStructure>) {
            const TStructure object{};
            auto tuple = tuple_from_struct(object, field_policy::reference{});
            std::size_t count{0};
            for_each(tuple, [&](auto &&field) {
                using type_t = std::decay_t<decltype(field)>;
                count += compute_field_count_recursive<type_t>();
            });
            return count;
        } else {
            return 1u;
        }
    }

}

/*! @} */
