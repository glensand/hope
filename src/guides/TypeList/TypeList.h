//------------------------------------------------------------------------------
// TypeList.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 30.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "customtraits/IsSame.h"
#include <type_traits>
#include <algorithm>
#include <array>

template <typename T>
struct TypeHolder
{
    using Type = T;
};

template <typename... Types>
class TypeList
{
    
};

template <typename... Ts>
constexpr size_t Size(TypeList<Ts...>)
{
    return sizeof...(Ts);
}

template<typename T, typename... Ts>
constexpr TypeHolder<T> Front(TypeList<T, Ts...>)
{
    return {};
}

template<typename T, typename... Ts>
constexpr TypeList<Ts...> PopFront(TypeList<T, Ts...>)
{
    return {};
}

template <typename T, typename... Ts>
constexpr TypeList<T, Ts...> PushFront(TypeList<Ts...>, TypeHolder<T>)
{
    return {};
}

template <typename T, typename... Ts>
constexpr TypeHolder<T> Back(TypeList<Ts..., T>)
{
    return {};
}

template <typename T, typename... Ts>
constexpr bool Contains(TypeList<Ts...>)
{
    return (... || IsSameV<T, Ts>);
}

template <typename T, typename... Ts>
constexpr bool Contains14(TypeList<Ts...>)
{
    constexpr bool bs[] = { IsSameV<T, Ts>... };
    bool result = false;
    for(bool b : bs)
        result |= b;

    return result;
}

template <typename T, typename... Ts>
constexpr size_t Find(TypeList<Ts...> list)
{
    constexpr bool bs[] = { IsSameV<T, Ts>... };
    constexpr size_t size = Size(list);

    for(size_t i = 0; i < size; ++i)
    {
        if (bs[i])
            return i;
    }
    return size;
}

template <typename T, typename... Ts>
constexpr size_t Find20(TypeList<Ts...>)
{
    constexpr bool bs[] = { IsSameV<T, Ts>... };
    return std::find(bs, true) - bs;
}

template <template <typename...> typename F, typename... Ts>
constexpr size_t FindIf(TypeList<Ts...>)
{
    constexpr bool bs[] = { F<Ts>::value... };
    return std::find(bs, true) - bs;
}

// TODO:: maybe it is good idea to use value based approach every were
template <template <typename...> typename F, typename... Ts>
constexpr bool AllOf(TypeList<Ts...>)
{
    return (... && F<Ts>::value);
}

template <template <typename...> typename F, typename... Ts>
constexpr bool AnyOf(TypeList<Ts...>)
{
    return (... || F<Ts>::value);
}

template <template <typename...> typename F, typename... Ts>
constexpr bool NoneOf(TypeList<Ts...> list)
{
    return !AnyOf<F>(list);
}

namespace Detail
{
    template <typename Is>
    struct Get;

    template <size_t... Is>
    struct Get<std::index_sequence<Is...>>
    {
        template <typename T>
        static constexpr T Extractor(decltype(Is, static_cast<void*>(0))..., T*, ...);
    };
}

template<size_t I, typename... Ts>
constexpr auto GetNthType(TypeList<Ts...>)
{
    return TypeHolder<decltype(
        Detail::Get<std::make_index_sequence<I>>::Extractor((Ts*)(0)...))>{};
}

template<size_t I, typename... Ts>
using NthType = typename decltype(GetNthType<I>(TypeList<Ts>{}))::Type;

template <typename T, typename... Ts>
constexpr size_t IndexOf(TypeList<Ts...>)
{
    constexpr bool bs[] = { IsSameV<T, Ts>... };
    return std::count_if(bs, true);
}

template <typename... Ts>
constexpr size_t LargestTypeIndex(TypeList<Ts...>)
{
    constexpr std::array<std::size_t, sizeof...(Ts)> sizes{ sizeof(Ts)... };
    constexpr auto it =  std::max_element(std::begin(sizes), std::end(sizes));
    return std::distance(std::begin(sizes), it);
}

template <typename... Ts>
constexpr auto LargestType(TypeList<Ts...> list)
{
    constexpr auto largestTypeIndex = LargestTypeIndex(list);
    return GetNthType<largestTypeIndex>(list);
}

template <typename... Ts>
constexpr TypeList<Ts...> MakeTypeList(TypeHolder<Ts>&&...)
{
    return {};
}