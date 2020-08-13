//------------------------------------------------------------------------------
// TupleFromStruct.h
// type_list like tuple
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 15.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "tuple/flat_tuple.h"
#include "tuple/detect_fields_count.h"

#include "typelist/type_list.h"

namespace hope {
    namespace detail::unsafe {

        template <typename T>
        constexpr T construct_helper() noexcept {
            return {};
        }

        template <std::size_t I>
        struct any_convertible {

            std::size_t* types;

            constexpr any_convertible(std::size_t* types)
                : types(types) { }

            constexpr void assign(std::size_t type) const noexcept {
                types[0] = type;
            }

            template <typename T>
            constexpr operator T () const noexcept {
                //constexpr auto type = find<std::decay_t<T>>(predefinedtypes::IntegralTypes);
                //assign(type);
                return construct_helper<T>();
            }
        };

        template<typename... Ts>
        constexpr auto tuple_from_type_list(hope::type_list<Ts...>) {
            return flat_tuple<Ts...>{};
        }

        template <std::size_t N, typename T, std::size_t... Is>
        constexpr auto extract_indexes(const T&, std::index_sequence<Is...> sequence) {
            constexpr auto length = size(sequence);
            std::size_t type_ids[length];
            constexpr T object{ any_convertible<Is>(type_ids)... };
            return 0;
            //  return MakeTypeList(GetNthType<typeIds[Is]>(PreDefinedTypes::IntegralTypes)...);
        }
    }
    template <typename T>
    constexpr auto tuple_from_struct_unsafe(const T& object) {
        // what the hall is it?? 
        //constexpr auto fieldsCountHall = DetectFieldsCount(object);

        constexpr auto fieldsCount = detect_fields_count(T{});

        //    constexpr auto typeList = Detail::Unsafe::ExtractIndexes<0>(T{}, std::make_index_sequence<fieldsCount>());
           /* auto tuple = Detail::Unsafe::TupleFromTypeList(typeList);
            reinterpret_cast<T&>(tuple) = object;
            return tuple;*/
    }
}
