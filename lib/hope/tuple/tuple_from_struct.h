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

    /**
     * \brief Creates tuple from the given struct, type of each field of the result tuple depends on specified field policy (value or reference)
     * \tparam T type of the passed struct
     * \tparam FieldPolicy Policy of the struct's fields which have to be used to determine result type of each field of the result tuple
     * \param object instance of the structure to be converted
     * \param p just used to determine field policy (type)
     * \return created tuple
     */
    template <typename T, typename FieldPolicy = field_policy::value>
    constexpr auto tuple_from_struct(T&& object, FieldPolicy p = FieldPolicy{}) {
        constexpr auto fields_count = detect_fields_count<std::decay_t<T>>();
        return detail::generated::tuple_from_struct(std::forward<T>(object), detail::Int<fields_count>{}, FieldPolicy{});
    }

}
