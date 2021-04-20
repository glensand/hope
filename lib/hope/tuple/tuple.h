/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <type_traits>
#include <ostream>

namespace hope {

    template <typename... Ts>
    class tuple;

    template<typename Head, typename... Tail>
    class tuple<Head, Tail...> {
    public:
        tuple() = default;

        template <typename HeadRef, typename... TailRef,
            typename = std::enable_if_t<std::is_same_v<HeadRef, Head>
            && std::is_same_v<TailRef, Tail>...>>
            tuple(Head&& head, tuple<Tail...>&& tail)
            : m_head(std::forward<Head>(head))
            , m_tail(std::forward<Tail>(tail)...) { }

        template <typename HeadRef, typename... TailRef,
            typename = std::enable_if_t<std::is_same_v<HeadRef, Head>
            && std::is_same_v<TailRef, Tail>...>>
            tuple(Head&& head, Tail&&... tail)
            : m_head(std::forward<Head>(head))
            , m_tail(std::forward<Tail>(tail)...) {}

        Head& head() {
            return m_head;
        }

        const Head& head() const {
            return m_head;
        }

        tuple<Tail...>& tail() {
            return m_tail;
        }

        const tuple<Tail...>& tail() const {
            return m_tail;
        }
    private:
        Head            m_head;
        tuple<Tail...>  m_tail;
    };

    template <>
    class tuple<> {

    };

    namespace detail
    {
        template <unsigned N>
        struct tuple_get {
            template <typename Head, typename... Tail>
            static auto apply(const tuple<Head, Tail...>& tuple) {
                return tuple_get<N - 1>::apply(tuple.GetTail());
            }
        };
    
        template <>
        struct tuple_get<0> {
            template<typename Head, typename... Tail>
            static const Head& apply(const tuple<Head, Tail...>& tuple) {
                return tuple.GetHead();
            }
        };
    
        inline void print_tuple(std::ostream& stream, tuple<> const&, bool isFirst = true) {
            stream << (isFirst ? '(' : ')');
        }
    
        template <typename Head, typename... Tail>
        void print_tuple(std::ostream& stream, tuple<Head, Tail...> const& tuple, bool isFirst = true) {
            stream << '(';
            stream << tuple.GetHead();
            print_tuple(stream, tuple.GetTail(), false);
        }
    }

    template <unsigned N, typename... Types>
    auto get(const tuple<Types...>& tuple) {
        return detail::tuple_get<N>::apply(tuple);
    }

    template <typename... Ts>
    auto make_tuple(Ts&&... elems) {
        return tuple<std::decay_t<Ts>...>(std::forward<Ts>(elems)...);
    }

    template <typename... Ts>
    std::ostream& operator<<(std::ostream& stream, tuple<Ts...> tuple) {
        detail::print_tuple(stream, tuple);
        return stream;
    }
}
