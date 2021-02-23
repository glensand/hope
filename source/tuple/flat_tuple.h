/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <utility>

#include "typelist/type_list.h"

namespace hope {
    namespace detail {

        template<typename T, size_t I>
        struct indexed_value {
            constexpr indexed_value() = default;
            constexpr indexed_value(indexed_value&&) = default;
            constexpr indexed_value(const indexed_value&) = default;

            ~indexed_value() = default;

            template<typename Vt,
                typename = std::is_constructible<T, Vt>>
                explicit constexpr indexed_value(Vt&& valueRef)
                : value(std::forward<Vt>(valueRef)) { }

            T value;
        };
    }

    template <typename, typename... Ts>
    class flat_tuple_impl;

    template <typename... Ts >
    using flat_tuple = flat_tuple_impl < std::make_index_sequence < size(type_list<Ts...>{}) > , Ts... > ;

    template<std::size_t... Is, typename... Ts>
    class flat_tuple_impl<std::index_sequence<Is...>, Ts...> final : public detail::indexed_value <Ts, Is>... {
    public:

        constexpr flat_tuple_impl() = default;
        constexpr flat_tuple_impl(const flat_tuple_impl&) = default;
        constexpr flat_tuple_impl(flat_tuple_impl&&) = default;

        ~flat_tuple_impl() = default;

        template <typename... VTs,
            typename = std::enable_if_t<std::is_same_v<type_list<VTs...>, type_list<Ts...>>>>
            constexpr flat_tuple_impl(VTs&&... elems) noexcept
            : detail::indexed_value<VTs, Is>(std::forward<VTs>(elems))...
        { }

        template <typename T, typename NativeT = std::decay_t<T>>
        [[nodiscard]] constexpr decltype(auto)
        get() noexcept {
            static_assert(contains<T>(types));
            return get_impl < find<NativeT>(types), NativeT >;
        }

        template <typename T, typename NativeT = std::decay_t<T>>
        [[nodiscard]] constexpr const NativeT& 
        get() const noexcept {
            static_assert(contains<T>(types));
            return get_impl<find<NativeT>(types), NativeT>();
        }

        template <size_t N>
        [[nodiscard]] constexpr decltype(auto)
        get() noexcept {
            static_assert(tuple_size > N);
            using NativeType = std::decay_t<typename decltype(get_nth_type<N>(types))::Type>;
            return get_impl<N, NativeType>();
        }

        template <size_t N>
        [[nodiscard]] constexpr decltype(auto)
        get() const noexcept {
            static_assert(tuple_size > N);
            using NativeType = std::decay_t<typename decltype(get_nth_type<N>(types))::Type >;
            return get_impl<N, NativeType>();
        }

        friend constexpr std::ostream& operator<< (std::ostream& stream, const flat_tuple<Ts...>& tuple) {
            print_impl(stream, tuple, std::make_index_sequence<tuple_size>());
            return stream;
        }

        template <typename F>
        friend constexpr void for_each(const flat_tuple<Ts...>& tuple, F&& f) {
            (f(tuple.template get<Is>()), ...);
        }
    private:

        template<std::size_t... VIs>
        friend void print_impl(std::ostream& stream, const flat_tuple<Ts...>& tuple, std::index_sequence<VIs...>) {
            stream << "{ ";
            ((stream << (VIs == 0 ? "" : ", ") << tuple.template get<VIs>()), ...);
            stream << " };";
        }

        template<std::size_t N, typename NativeT>
        [[nodiscard]] constexpr decltype(auto)
        get_impl() const noexcept {
            return static_cast<const NativeT&>(
                static_cast<const detail::indexed_value < NativeT, N >&> (*this).value
                );
        }

        template<std::size_t N, typename NativeT>
        constexpr decltype(auto)
        get_impl() noexcept {
            return static_cast<NativeT&>(
                static_cast<detail::indexed_value < NativeT, N >&> (*this).value
                );
        }

        constexpr static type_list<Ts...>   types{ };
        constexpr static std::size_t        tuple_size{ size(types) };
    };

    template <typename... Ts>
    constexpr auto make_flat_tuple(Ts... args) {
        return flat_tuple<Ts...>(std::forward<Ts>(args)...);
    }

}