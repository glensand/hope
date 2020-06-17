//------------------------------------------------------------------------------
// TupleFromStruct.h
// TypeList like tuple
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 15.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "tuple/FlatTuple.h"
#include "tuple/DetectFieldsCount.h"

#include "typelist/TypeList.h"
#include "typelist/integraltypes.h"

namespace Detail
{

    namespace Unsafe
    {
        template <typename T>
        constexpr T ConstructHelper() noexcept
        {
            return {};
        }

        template <std::size_t I>
        struct AnyConvertible
        {

            std::size_t* Types;
            constexpr AnyConvertible(std::size_t* types)
                : Types(types)
            {
                
            }

            constexpr void Assign(std::size_t type) const noexcept
            {
                Types[0] = type;
            }

            template <typename T>
            constexpr operator T () const noexcept
            {
                constexpr auto type = Find<std::decay_t<T>>(PreDefinedTypes::IntegralTypes);
                Assign(type);
                return ConstructHelper<T>();
            }
        };

        template<typename... Ts>
        constexpr auto TupleFromTypeList(TypeList<Ts...>)
        {
            return FlatTuple<Ts...>{};
        }

        template <std::size_t N, typename T, std::size_t... Is>
        constexpr auto ExtractIndexes(const T&, std::index_sequence<Is...> sequence)
        {
            constexpr auto length = size(sequence);
            std::size_t typeIds[length];
            constexpr T fakeObject{ AnyConvertible<Is>( typeIds )... };
            return 0;
          //  return MakeTypeList(GetNthType<typeIds[Is]>(PreDefinedTypes::IntegralTypes)...);
        }

        
    }
   
}

template <typename T>
constexpr auto TupleFromStructUnsafe(const T& object)
{
    // what the hall is it?? 
    //constexpr auto fieldsCountHall = DetectFieldsCount(object);

    constexpr auto fieldsCount = DetectFieldsCount(T{});

//    constexpr auto typeList = Detail::Unsafe::ExtractIndexes<0>(T{}, std::make_index_sequence<fieldsCount>());
   /* auto tuple = Detail::Unsafe::TupleFromTypeList(typeList);
    reinterpret_cast<T&>(tuple) = object;
    return tuple;*/
}