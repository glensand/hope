//------------------------------------------------------------------------------
// generated.h
// TypeList like tuple
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 17.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "tuple/FlatTuple.h"
#include "tuple/DetectFieldsCount.h"

#include "typelist/TypeList.h"

namespace Detail
{

    template<std::size_t I>
    using Int = std::integral_constant<std::size_t, I>;

    namespace Generated
    {
        template <typename T>
        constexpr auto TupleFromStruct(const T& object, Int<1>)
        {
            auto& [a] = object;
            return MakeFlatTuple(a);
        }

        template <typename T>
        constexpr auto TupleFromStruct(const T& object, Int<2>)
        {
            auto& [a, b] = object;
            return MakeFlatTuple(a, b);
        }

        template <typename T>
        constexpr auto TupleFromStruct(const T& object, Int<3>)
        {
            auto& [a, b, c] = object;
            return MakeFlatTuple(a, b, c);
        }

        template <typename T>
        constexpr auto TupleFromStruct(const T& object, Int<4>)
        {
            auto& [a, b, c, d] = object;
            return MakeFlatTuple(a, b, c, d);
        }

        template <typename T>
        constexpr auto TupleFromStruct(const T& object, Int<5>)
        {
            auto& [a, b, c, d, e] = object;
            return MakeFlatTuple(a, b, c, d, e);
        }

        template <typename T>
        constexpr auto TupleFromStruct(const T& object, Int<6>)
        {
            auto& [a, b, c, d, e, f] = object;
            return MakeFlatTuple(a, b, c, d, e, f);
        }
    }
   
}

template <typename T>
constexpr auto TupleFromStruct(const T& object)
{
    constexpr auto fieldsCount = DetectFieldsCount(T{});
    return Detail::Generated::TupleFromStruct(object, Detail::Int<fieldsCount>{});
}