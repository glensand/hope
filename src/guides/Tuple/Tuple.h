//------------------------------------------------------------------------------
// Tuple.h
//
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 30.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include <type_traits>
#include <ostream>

template <typename... Ts>
class Tuple;

template<typename Head, typename... Tail>
class Tuple<Head, Tail...>
{
public:
    Tuple() = default;

    template <typename HeadRef, typename... TailRef,
    typename = std::enable_if_t<std::is_same_v<HeadRef, Head>
    && std::is_same_v<TailRef, Tail>...>>
    Tuple(Head&& head, Tuple<Tail...>&& tail)
        : m_head(std::forward<Head>(head))
        , m_tail(std::forward<Tail>(tail)...)
    {
    }

    template <typename HeadRef, typename... TailRef,
    typename = std::enable_if_t<std::is_same_v<HeadRef, Head>
    && std::is_same_v<TailRef, Tail>...>>
        Tuple(Head&& head, Tail&&... tail)
        : m_head(std::forward<Head>(head))
        , m_tail(std::forward<Tail>(tail)...)
    {
    }

    Head& GetHead()
    {
        return m_head;
    }

    const Head& GetHead() const
    {
        return m_head;
    }

    Tuple<Tail...>& GetTail()
    {
        return m_tail;
    }

    const Tuple<Tail...>& GetTail() const
    {
        return m_tail;
    }

private:
    Head            m_head;
    Tuple<Tail...>  m_tail;
};

template <>
class Tuple<>
{
    
};

namespace Detail
{
    template <unsigned N>
    struct TupleGet
    {
        template <typename Head, typename... Tail>
        static auto Apply(const Tuple<Head, Tail...>& tuple)
        {
            return TupleGet<N - 1>::Apply(tuple.GetTail());
        }
    };

    template <>
    struct TupleGet<0>
    {
        template<typename Head, typename... Tail>
        static const Head& Apply(const Tuple<Head, Tail...>& tuple)
        {
            return tuple.GetHead();
        }
    };

    inline void PrintTuple(std::ostream& stream, Tuple<> const&, bool isFirst = true)
    {
        stream << (isFirst ? '(' : ')');
    }

    template <typename Head, typename... Tail>
    void PrintTuple(std::ostream& stream, Tuple<Head, Tail...> const& tuple, bool isFirst = true)
    {
        stream << '(';
        stream << tuple.GetHead();
        PrintTuple(stream, tuple.GetTail(), false);
    }
}


template <unsigned N, typename... Types>
auto Get(const Tuple<Types...>& tuple)
{
    return Detail::TupleGet<N>::Apply(tuple);
}

template <typename... Ts>
auto MakeTuple(Ts&&... elems)
{
    return Tuple<std::decay_t<Ts>...>(std::forward<Ts>(elems)...);
}

template <typename... Ts>
std::ostream& operator<<(std::ostream& stream, Tuple<Ts...> tuple)
{
    Detail::PrintTuple(stream, tuple);
    return stream;
}