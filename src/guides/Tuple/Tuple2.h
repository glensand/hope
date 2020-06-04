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
    constexpr Tuple(VTs&&... elems)
    {
        // TODO:: implement for_each for type list and use it here
    }

    template <typename T, typename NativeT = Decay<T>>
    constexpr std::enable_if_t<Contains<T>(TypeList<Ts...>{}),
    NativeT&> Get()
    {
        return static_cast<NativeT&>(
            static_cast<Detail::JustValue<T>&>(*this).Value
            );
    }

    template <typename T, typename NativeT = Decay<T>>
    constexpr std::enable_if_t<Contains<T>(TypeList<Ts...>{}),
    const NativeT&> Get() const
    {
        return static_cast<const NativeT&>(
            static_cast<const Detail::JustValue<T>&>(*this).Value
            );
    }

    friend std::ostream& operator<< (std::ostream& stream, Tuple<Ts...> tuple)
    {
        // TODO:: implement for_each
        return stream;
    }

private:
    UniqueTypes<Ts...>  m_uniqueChecker;
};

