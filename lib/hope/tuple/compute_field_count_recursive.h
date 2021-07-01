/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
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

namespace hope {

    namespace detail {

        template<typename T>
        constexpr std::enable_if_t<!is_user_defined_type_v<T>, std::size_t>
        compute_via_type_list() {
            return 1;
        }

        template<typename T>
        constexpr std::enable_if_t<is_user_defined_type_v<T>, std::size_t>
        compute_via_type_list() {
            constexpr auto fields_count = detect_fields_count<T>();
            constexpr auto types = detail::extract_types<T>(std::make_index_sequence<fields_count>());
            std::size_t count{ 0 };
            for_each(types, [&] (auto field) {
                using type_t = typename decltype(field)::Type;
                count += compute_via_type_list<type_t>();
            });
            
            return count;
        }

        template<typename T>
        std::enable_if_t<!is_user_defined_type_v<T>, std::size_t>
        compute_via_tuple() {
            return 1;
        }

        template<typename T>
        std::enable_if_t<is_user_defined_type_v<T>, std::size_t>
        compute_via_tuple() {
            const T object{ };
            auto tuple = tuple_from_struct(object, field_policy::reference{ });
            std::size_t count{ 0 };
            for_each(tuple, [&](auto &&field) {
                using type_t = std::decay_t<decltype(field)>;
                count += compute_via_tuple<type_t>();
            });
            return count;
        }

    }

    template<typename T>
    constexpr std::size_t compute_field_count_recursive_constexpr() {
        return detail::compute_via_type_list<T>();
    }

    template<typename T>
    constexpr std::size_t compute_field_count_recursive() {
            return detail::compute_via_tuple<T>();
    }

}
