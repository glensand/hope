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
    constexpr Tuple(T&& front, const Tuple< VTs...>& tuple) noexcept
        : Detail::JustValue<T>(std::forward<T>(front))
        //, Detail::JustValue<VTs>(static_cast<const Detail::JustValue<VTs&>>(tuple).Value)...
        , Detail::JustValue<VTs>(tuple.Get<VTs>())...
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

    template <size_t N,
    typename = std::enable_if < (Size(TypeList<Ts...>{}) > N) >>
    constexpr decltype(auto) Get() const noexcept 
    {
        using NativeType = Decay<typename decltype(GetNthType<N>(TypeList<Ts...>{}))::Type >;
        return Get<NativeType>();
    }

    friend constexpr std::ostream& operator<< (std::ostream& stream, const Tuple<Ts...>& tuple)
    {
        constexpr auto length = Size(TypeList<Ts...>{});
        Print(stream, tuple, std::make_index_sequence<length>{});
        return stream;
    }

private:

    template<std::size_t... Is>
    friend void Print(std::ostream& stream, const Tuple<Ts...>& tuple, std::index_sequence<Is...>)
    {
        stream << "{ ";
        ((stream << (Is == 0 ? "" : ", ") << tuple.Get<Is>()), ...);
        stream << " };";
    }

    UniqueTypes<Ts...>  m_uniqueChecker;
};

template<typename T, typename... Ts>
auto PushFront(T&& element, const Tuple<Ts...>& tuple)
{
    return Tuple<Decay<T>, Decay<Ts>...>(std::forward<T&&>(element), tuple);
}

template<typename T, typename... Ts>
auto TuplePushBack(T&& element, const Tuple<Ts...>& tuple)
{
    return Tuple<Decay<Ts>..., Decay<T>>(std::forward<T&&>(element), tuple);
}

