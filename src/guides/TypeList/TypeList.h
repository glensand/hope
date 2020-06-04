//------------------------------------------------------------------------------
// TypeList.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 30.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include <CustomTraits/IsSame.h>
#include <type_traits>

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

static_assert(Contains<int>(TypeList<int, bool, float, double>{}));
static_assert(!Contains<int>(TypeList<bool, float, double>{}));

template <typename T, typename... Ts>
constexpr bool Contains14(TypeList<Ts...>)
{
    constexpr bool bs[] = { IsSameV<T, Ts>... };
    bool result = false;
    for(bool b : bs)
        result |= b;

    return result;
}

static_assert(Contains14<int>(TypeList<int, bool, float, double>{}));
static_assert(!Contains14<int>(TypeList<bool, float, double>{}));

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

static_assert(Find<int>(TypeList<int, bool, float, double>{}) == 0);
static_assert(Find<unsigned>(TypeList<int, bool, float, double>{}) == 4);

template <template <typename...> typename F, typename... Ts>
constexpr size_t FindIf(TypeList<Ts...> list)
{
    // TODO:: explore and...
    constexpr bool bs[] = { F<Ts>::value... };
    return 0;
}

// TODO:: maybe it is good idea to use value based approach every were
template <template <typename...> typename F, typename... Ts>
constexpr bool AllOf(TypeList<Ts...>)
{
    return (... && F<Ts>::value);
}

static_assert(AllOf<std::is_pointer>(TypeList<int*, bool*, float*, double*>{}));
static_assert(!AllOf<std::is_pointer>(TypeList<int, bool, float, double>{}));
static_assert(AllOf<std::is_integral>(TypeList<int, unsigned, long, long long, short>{}));

template <template <typename...> typename F, typename... Ts>
constexpr bool AnyOf(TypeList<Ts...>)
{
    return (... || F<Ts>::value);
}

static_assert(AnyOf<std::is_pointer>(TypeList<int, bool*, float*, double*>{}));

template <template <typename...> typename F, typename... Ts>
constexpr bool NoneOf(TypeList<Ts...> list)
{
    return !AnyOf<F>(list);
}