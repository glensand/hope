/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/tuple/generated.h"
#include "hope/tuple/detect_fields_count.h"

namespace hope {

    template <typename T, typename BitPolicy = field_policy::byte>
    constexpr auto tuple_from_struct(const T& object, BitPolicy bit_policy = BitPolicy{ }) {
        constexpr auto fields_count = detect_fields_count(T{ }, bit_policy);
        return detail::generated::tuple_from_struct(object, detail::Int<fields_count>{});
    }

    template <typename T>
    constexpr auto ref_tuple_from_struct(T&& object) {
        constexpr auto fields_count = detect_fields_count(std::decay_t<T>{ }, field_policy::byte{});
        return detail::generated::tuple_from_struct(std::forward<T>(object), detail::Int<fields_count>{}, field_policy::reference{});
    }

}
