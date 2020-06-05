//------------------------------------------------------------------------------
// Tuple.h
// TypeList like tuple
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 30.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "CustomTraits/UniqueTypes.h"
#include "TypeList/TypeList.h"
#include "CustomTraits/Decay.h"

namespace Detail
{
    template<typename T>
    struct JustValue
    {
        T Value;
    };
}

template<typename... Ts>
class Tuple final : public Detail::JustValue<Ts>...
{
public:

    template <typename... VTs>
    constexpr Tuple(VTs&&... elems) noexcept
    {
        // TODO:: implement for_each for type list and use it here
    }

    template <typename T, typename NativeT = Decay<T>>
    constexpr std::enable_if_t<Contains<T>(TypeList<Ts...>{}),
    NativeT&> Get() noexcept
    {
        return static_cast<NativeT&>(
            static_cast<Detail::JustValue<T>&>(*this).Value
            );
    }

    template <typename T, typename NativeT = Decay<T>>
    constexpr std::enable_if_t<Contains<T>(TypeList<Ts...>{}),
    const NativeT&> Get() const noexcept
    {
        return static_cast<const NativeT&>(
            static_cast<const Detail::JustValue<T>&>(*this).Value
            );
    }

    template <size_t N,
    typename = std::enable_if<(Size(TypeList<Ts...>{}) > N)>>
    constexpr decltype(auto) Get() noexcept
    {
        using NativeType = Decay<typename decltype(GetNthType<N>(TypeList<Ts...>{}))::Type>;
        return Get<NativeType>();
    }

    friend std::ostream& operator<< (std::ostream& stream, Tuple<Ts...> tuple)
    {
        // TODO:: implement for_each
        return stream;
    }

    template<typename T>
    Tuple<T, Ts...> PushFront(T&&)
    {
        this;
        return {};
    }

    template<typename T>
    Tuple<T, Ts...> PushBack(T&&)
    {
        this;
        return {};
    }

    template <template <typename...> typename F>
    constexpr void ForEach()
    {
        F(static_cast<Detail::JustValue<Ts>&>(*this)...);
        //// TODO:: explore and...
        //constexpr bool bs[] = { F<Ts>::value... };
        //return std::find(bs, true) - bs;
    }

private:
    UniqueTypes<Ts...>  m_uniqueChecker;
};

