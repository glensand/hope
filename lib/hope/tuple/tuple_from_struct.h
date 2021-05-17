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
     * \brief Creates tuple from the given struct, copies each field ot the object to the related one of the result tuple
     * \tparam T type of the passed struct
     * \tparam BitPolicy Policy of the struct's fields which have to be used to compute fields count of the structure
     * if field_policy::byte is used, the compilation will be a little faster then field_policy::bit, but this one is not suitable
     * for structures with fields of small bit size (i mean if structure has bit field which size less then 1 byte,
     * compilation with field_policy::byte will be failed
     * \param object instance of the structure to be converted
     * \param bit_policy just used to determine bit policy (type)
     * \return created tuple
     */
    template <typename T, typename... Ps>
    constexpr auto tuple_from_struct(const T& object, Ps...) {
        using map = tuple_policy::tuple<Ps...>;
        constexpr auto fields_count = detect_fields_count(T{ }, 
            map::template has<tuple_policy::bit>() ? std::true_type{} : std::false_type{});

        return detail::generated::tuple_from_struct(object, detail::Int<fields_count>{});
    }

    /**
     * \brief Creates tuple of references to the given structure fields
     * \tparam T object type to be processed
     * \param object just instance
     * \return tuple with references to the structure's fields
     */
    template <typename T>
    constexpr auto ref_tuple_from_struct(T&& object) {
        constexpr auto fields_count = detect_fields_count(std::decay_t<T>{ }, field_policy::byte{});
        return detail::generated::tuple_from_struct(std::forward<T>(object), detail::Int<fields_count>{}, field_policy::reference{});
    }

}
