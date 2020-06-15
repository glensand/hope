//------------------------------------------------------------------------------
// typelistsort.h

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
    struct SortHelper
    {
        constexpr SortHelper(std::size_t index, std::size_t size) noexcept
            : TypeIndex(index)
            , TypeSize(size)
        {
            
        }

        constexpr bool operator<(const SortHelper& r) const
        {
            return TypeSize < r.TypeSize;
        }

        constexpr bool operator>(const SortHelper& r) const
        {
            return TypeSize > r.TypeSize;
        }

        std::size_t     TypeIndex;
        std::size_t     TypeSize;
    };

    template <typename T>
    constexpr void Swap(T&& l, T&& r)
    {
        auto temp{ r };
        r = static_cast<T&&>(l);
        l = static_cast<T&&>(temp);
    }

    template<std::size_t... Is, typename... Ts>
    constexpr auto SortImpl(std::index_sequence<Is...> sequence, TypeList<Ts...> list)
    {
        constexpr auto listSize = Size(list);
        std::array<SortHelper, listSize> sizes{ SortHelper(Is, sizeof(Ts))...};

        // should be replaced with std::sort in cxx20
        for(size_t i{ 0 }; i < listSize; ++i)
        {
            auto it = std::max_element(std::begin(sizes) + i, std::end(sizes));
            size_t pos = std::distance(std::begin(sizes), it);
            if(i != pos)
                Swap(sizes[i], sizes[pos]);
        }

        return sizes;
    }

    template <typename... Ts>
    constexpr TypeList<Ts...> MakeTypeList(TypeHolder<Ts>&&...)
    {
        return {};
    }

    template<std::size_t... Is, typename... Ts>
    constexpr auto SortTypeList(std::index_sequence<Is...> sequence, TypeList<Ts...> list)
    {
        constexpr auto sortedIndexArray = Detail::SortImpl(sequence, list);
        return MakeTypeList(GetNthType<sortedIndexArray[Is].TypeIndex>(list)...);
    }

}

template <typename... Ts>
constexpr auto Sort(TypeList<Ts...> list)
{
    constexpr auto sortedIndexArray = Detail::SortTypeList(std::make_index_sequence<Size(list)>(), list);
    return sortedIndexArray;
}