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

#include "typelist/TypeList.h"

namespace Detail
{
    //template <std::size_t N, std::size_t I>
    //struct AnyConvertible
    //{
    //    template <typename T>
    //    T& operator()(std::array<std::size_t, N>& types)
    //    {
    //        types[I] = Find<std::decay_t<T>>(PreDefinedTypes::IntegralTypes);
    //        return {};
    //    }
    //};

    template <std::size_t I>
    struct AnyConvertible
    {
        std::size_t fake;
        template <typename T>
        /*constexpr */operator T& () const noexcept;
    };

    template <typename T, std::size_t... Is>
    constexpr auto IsConstructibleN(const T&, std::index_sequence<Is...>)
    {
        return std::is_constructible_v < T, decltype(AnyConvertible<Is>{})...>;
    }

    template <typename T, std::size_t... Is>
    constexpr std::size_t DetectFieldsCountImpl(const T& object, std::index_sequence<Is...> sequence)
    {
        bool bs[] = { IsConstructibleN(object, std::make_index_sequence<Is>())... };

        // since cxx20... msvc sucks
        //return std::distance(std::begin(bs), std::find(
        //    std::begin(bs), std::end(bs), false)
        //);

        for (std::size_t i{ 0 }; i < size(sequence); ++i)
        {
            if (!bs[i]) return i;
        }

        return size(sequence);
    }

}

template <typename T>
constexpr std::size_t DetectFieldsCount(const T& object)
{
    constexpr auto size = sizeof(T);
    return Detail::DetectFieldsCountImpl(object, std::make_index_sequence<size>());
}