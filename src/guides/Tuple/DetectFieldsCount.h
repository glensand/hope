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

namespace Detail
{

    template <std::size_t I>
    struct AnyConvertible
    {
        std::size_t fake;
        template <typename T>
        constexpr operator T& () const noexcept;
    };

    template <typename T, std::size_t... Is>
    constexpr auto IsConstructibleN(const T&, std::index_sequence<Is...>)
        ->decltype(T{AnyConvertible<Is>{}...}, bool())
    {
        return true;
    }

    template <typename T, std::size_t... Is>
    constexpr auto IsConstructibleN(const T&, ...)
    {
        return false;
    }

    template <typename T, std::size_t... Is>
    constexpr std::size_t DetectFieldsCountImpl(const T& object, std::index_sequence<Is...> sequence)
    {
        bool bs[] = { IsConstructibleN(object, std::make_index_sequence<Is>())... };

        // since cxx20... msvc sucks
        //return std::distance(std::begin(bs), std::find(
        //    std::begin(bs), std::end(bs), false)
        //);

        for (std::size_t i{ 1 }; i < size(sequence); ++i)
        {
            if (!bs[i]) return i - 1;
        }

        return 0;
    }

}

template <typename T>
constexpr std::size_t DetectFieldsCount(const T& object)
{
    constexpr auto size = sizeof(T);
    return Detail::DetectFieldsCountImpl(object, std::make_index_sequence<size>());
}