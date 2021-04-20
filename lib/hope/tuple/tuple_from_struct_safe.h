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

    template <typename T, typename BitPolicy = field_policy_byte>
    constexpr auto tuple_from_struct(const T& object, BitPolicy has_bit_field = BitPolicy{ }) {
        constexpr auto fields_count = detect_fields_count(T{ }, has_bit_field);
        return detail::generated::tuple_from_struct(object, detail::Int<fields_count>{});
    }

}
