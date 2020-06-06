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
        constexpr JustValue() = default;
        constexpr JustValue(JustValue&&) = default;
        constexpr JustValue(const JustValue&) = default;

        ~JustValue() = default;

        template<typename Vt,
        typename = std::is_constructible<T, Vt>>
        explicit constexpr JustValue(Vt&& valueRef) /*noexcept(T(std::forward<Vt>(valueRef)))*/
            : Value(std::forward<Vt>(valueRef))
        {

        }

        T Value;
    };
}

template<typename... Ts>
class Tuple final : public Detail::JustValue<Ts>...
{
public:

    constexpr Tuple() = default;
    constexpr Tuple(const Tuple&) = default;
    constexpr Tuple(Tuple&&) = default;

    ~Tuple() = default;

    template <typename... VTs,
    typename = std::enable_if_t<std::is_same_v<TypeList<VTs...>, TypeList<Ts...>>>>
    constexpr Tuple(VTs&&... elems) noexcept
        : Detail::JustValue<VTs>(std::forward<VTs>(elems))...
    {
    }

    template <typename... VTs, typename T>
    constexpr Tuple(T&& front, Tuple< VTs...>&& tuple) noexcept
        : Detail::JustValue<T>(std::forward<T>(front))
        //, Detail::JustValue<VTs>(std::forward<VTs>(elems))...
    {
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

private:
    UniqueTypes<Ts...>  m_uniqueChecker;
};

template<typename T, typename... Ts>
Tuple<T, Ts...> PushFront(T&& element, Tuple<Ts...>&& tuple)
{
    
    return {};
}

template<typename T, typename... Ts>
Tuple<T, Ts...> PushBack(T&& element, Tuple<Ts...>&& tuple)
{
    
    return {};
}

