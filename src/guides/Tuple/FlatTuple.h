//------------------------------------------------------------------------------
// FlatTuple.h
// TypeList like tuple
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 30.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "TypeList/TypeList.h"
#include "CustomTraits/Decay.h"

namespace Detail
{
    template<typename T, size_t I>
    struct IndexedValue
    {
        constexpr IndexedValue() = default;
        constexpr IndexedValue(IndexedValue&&) = default;
        constexpr IndexedValue(const IndexedValue&) = default;

        ~IndexedValue() = default;

        template<typename Vt,
        typename = std::is_constructible<T, Vt>>
        explicit constexpr IndexedValue(Vt&& valueRef)
            : Value(std::forward<Vt>(valueRef))
        {

        }

        T Value;
    };
}

template <typename, typename... Ts>
class FlatTupleImpl;

template <typename... Ts >
using FlatTuple = FlatTupleImpl<std::make_index_sequence<Size(TypeList<Ts...>{})>, Ts...>;

template<std::size_t... Is, typename... Ts>
class FlatTupleImpl<std::index_sequence<Is...>, Ts...> final : public Detail::IndexedValue <Ts, Is>...
{
public:

    constexpr FlatTupleImpl() = default;
    constexpr FlatTupleImpl(const FlatTupleImpl&) = default;
    constexpr FlatTupleImpl(FlatTupleImpl&&) = default;

    ~FlatTupleImpl() = default;

    template <typename... VTs,
    typename = std::enable_if_t<std::is_same_v<TypeList<VTs...>, TypeList<Ts...>>>>
    constexpr FlatTupleImpl(VTs&&... elems) noexcept
        : Detail::IndexedValue<VTs, Is>(std::forward<VTs>(elems))...
    {
    }

    template <typename T, typename NativeT = Decay<T>>
    constexpr std::enable_if_t<Contains<T>(TypeList<Ts...>{}),
    NativeT&> Get() noexcept
    {
        return GetImpl < Find<NativeT>(TypeList<Ts...>{}), NativeT > ;
    }

    template <typename T, typename NativeT = Decay<T>>
    constexpr std::enable_if_t<Contains<T>(TypeList<Ts...>{}),
    const NativeT&> Get() const noexcept
    {
        return GetImpl<Find<NativeT>(TypeList<Ts...>{}), NativeT>();
    }

    template <size_t N,
    typename = std::enable_if<(Size(TypeList<Ts...>{}) > N)>>
    constexpr decltype(auto) Get() noexcept
    {
        using NativeType = Decay<typename decltype(GetNthType<N>(TypeList<Ts...>{}))::Type>;
        return GetImpl<N, NativeType>();
    }

    template <size_t N,
    typename = std::enable_if < (Size(TypeList<Ts...>{}) > N) >>
    [[nodiscard]] constexpr decltype(auto) Get() const noexcept 
    {
        using NativeType = Decay<typename decltype(GetNthType<N>(TypeList<Ts...>{}))::Type >;
        return GetImpl<N, NativeType>();
    }

    friend constexpr std::ostream& operator<< (std::ostream& stream, const FlatTuple<Ts...>& tuple)
    {
        constexpr auto length = Size(TypeList<Ts...>{});
        PrintImpl(stream, tuple, std::make_index_sequence<length>{});
        return stream;
    }

private:

    template<std::size_t... VIs>
    friend void PrintImpl(std::ostream& stream, const FlatTuple<Ts...>& tuple, std::index_sequence<VIs...>)
    {
        stream << "{ ";
        ((stream << (VIs == 0 ? "" : ", ") << tuple.template Get<VIs>()), ...);
        stream << " };";
    }

    template<std::size_t N, typename NativeT>
    [[nodiscard]] decltype(auto) GetImpl() const noexcept
    {
        return static_cast<const NativeT&>(
            static_cast<const Detail::IndexedValue < NativeT, N > & > (*this).Value
            );
    }

    template<std::size_t N, typename NativeT>
    decltype(auto) GetImpl() noexcept
    {
        return static_cast<NativeT&>(
            static_cast<Detail::IndexedValue < NativeT, N >&> (*this).Value
            );
    }
};

template <typename... Ts>
constexpr auto MakeFlatTuple(Ts... args)
{
    return FlatTuple<Ts...>(std::forward<Ts>(args)...);
}