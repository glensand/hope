/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/components/unique_types.h"
#include "hope/typelist/type_list.h"

#include <ostream>

namespace hope {

    namespace detail {

        template<typename T>
        struct just_value
        {
            constexpr just_value() = default;
            constexpr just_value(just_value&&) = default;
            constexpr just_value(const just_value&) = default;

            ~just_value() = default;

            template<typename Vt,
                typename = std::is_constructible<T, Vt>>
                explicit constexpr just_value(Vt&& valueRef) /*noexcept(T(std::forward<Vt>(valueRef)))*/
                : value(std::forward<Vt>(valueRef)) { }

            T value;
        };
    }

    template<typename... Ts>
    class flat_tuple_dummy final : public detail::just_value<Ts>... {
    public:
        constexpr flat_tuple_dummy() = default;
        constexpr flat_tuple_dummy(const flat_tuple_dummy&) = default;
        constexpr flat_tuple_dummy(flat_tuple_dummy&&) = default;

        ~flat_tuple_dummy() = default;

        template <typename... VTs,
            typename = std::enable_if_t<std::is_same_v<type_list<VTs...>, type_list<Ts...>>>>
            constexpr flat_tuple_dummy(VTs&&... elems) noexcept
            : detail::just_value<VTs>(std::forward<VTs>(elems))...
        { }

        template <typename... VTs, typename T>
        constexpr flat_tuple_dummy(T&& front, const flat_tuple_dummy< VTs...>& tuple) noexcept
            : detail::just_value<T>(std::forward<T>(front))
            , detail::just_value<VTs>(tuple.template get<VTs>())...
        {  }

        template <typename T, typename NativeT = std::decay_t<T>>
        constexpr std::enable_if_t < contains<T>(type_list<Ts...>{}), NativeT& >
        get() noexcept {
            return static_cast<NativeT&>(
                static_cast<detail::just_value<T>&>(*this).value
                );
        }

        template <typename T, typename NativeT = std::decay_t<T>>
        constexpr std::enable_if_t < contains<T>(type_list<Ts...>{}), const NativeT& >
        get() const noexcept {
            return static_cast<const NativeT&>(
                static_cast<const detail::just_value<T>&>(*this).value
                );
        }

        template <std::size_t N, typename = std::enable_if < (size(type_list<Ts...>{}) > N) >>
        constexpr decltype(auto)
        get() noexcept {
            using NativeType = std::decay_t<typename decltype(get_nth_type<N>(type_list<Ts...>{}))::Type > ;
            return get<NativeType>();
        }

        template <std::size_t N, typename = std::enable_if < (size(type_list<Ts...>{}) > N) >>
        constexpr decltype(auto)
        get() const noexcept {
            using NativeType = std::decay_t<typename decltype(get_nth_type<N>(type_list<Ts...>{}))::Type > ;
            return get<NativeType>();
        }

        friend constexpr std::ostream& operator<< (std::ostream& stream, const flat_tuple_dummy<Ts...>& tuple) {
            constexpr auto length = size(type_list<Ts...>{});
            print(stream, tuple, std::make_index_sequence<length>{});
            return stream;
        }

    private:
        template<std::size_t... Is>
        friend void print(std::ostream& stream, const flat_tuple_dummy<Ts...>& tuple, std::index_sequence<Is...>) {
            stream << "{ ";
            ((stream << (Is == 0 ? "" : ", ") << tuple.get<Is>()), ...);
            stream << " };";
        }

        unique_types<Ts...>  m_unique_checker;
    };

    template<typename T, typename... Ts>
    auto push_front(T&& element, const flat_tuple_dummy<Ts...>& tuple) {
        return flat_tuple_dummy<std::decay_t<T>, std::decay_t<Ts>...>(std::forward<T&&>(element), tuple);
    }

    template<typename T, typename... Ts>
    auto push_bask(T&& element, const flat_tuple_dummy<Ts...>& tuple) {
        return flat_tuple_dummy<std::decay_t<Ts>..., std::decay_t<T>>(std::forward<T&&>(element), tuple);
    }
}
