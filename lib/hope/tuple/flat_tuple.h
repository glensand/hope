/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <iostream>

#include "hope/typelist/type_list.h"

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

        template <typename, typename... Ts>
        class flat_tuple_impl;

        template<std::size_t... Is, typename... Ts>
        class flat_tuple_impl<std::index_sequence<Is...>, Ts...> : public indexed_value <Ts, Is>... {
            using self_t = flat_tuple_impl<std::index_sequence<Is...>, Ts...>;
            constexpr static type_list<std::decay_t<Ts>...> types{ }; // only clear types might be used, thus make it clear here

            template <std::size_t I>
            using native_t = std::decay_t<typename decltype(get_nth_type<I>(types))::Type>;
        protected:
            using flat_tuple = flat_tuple_impl<std::index_sequence<Is...>, Ts...>;
            constexpr flat_tuple_impl() = default;
            constexpr flat_tuple_impl(const flat_tuple_impl&) = default;
            constexpr flat_tuple_impl(flat_tuple_impl&&) = default;

            ~flat_tuple_impl() = default;

            template <typename... VTs>
            constexpr flat_tuple_impl(VTs&&... elems) noexcept
                : indexed_value<Ts, Is>(std::forward<VTs>(elems))...
            { }
        public:
            constexpr static std::size_t size{ size(types) };

            template <typename T, typename NativeT = std::decay_t<T>>
            [[nodiscard]] constexpr NativeT&
            get() noexcept {
                static_assert(contains<NativeT>(types));
                return get_impl < find<T>(types), T >;
            }

            template <typename T, typename NativeT = std::decay_t<T>>
            [[nodiscard]] constexpr const NativeT&
            get() const noexcept {
                static_assert(contains<NativeT>(types));
                return get_impl<find<NativeT>(types), NativeT>();
            }

            template <size_t N>
            [[nodiscard]] constexpr decltype(auto)
            get() noexcept {
                static_assert(size > N);
                using value_t = typename decltype(deduce_type<N>())::Type;
                return get_impl<N, value_t>();
            }

            template <size_t N>
            [[nodiscard]] constexpr decltype(auto)
            get() const noexcept {
                static_assert(size > N);
                using value_t = typename decltype(deduce_type<N>())::Type;
                return get_impl<N, value_t>();
            }

            friend constexpr std::ostream& operator<< (std::ostream& stream, const flat_tuple& tuple) {
                print_impl(stream, tuple, std::make_index_sequence<size>());
                return stream;
            }

            template <typename F>
            friend constexpr void for_each(const flat_tuple& tuple, F&& f) {
                (f(tuple.template get<Is>()), ...);
            }

            template <typename F>
            friend constexpr void for_each(flat_tuple& tuple, F&& f) {
                (f(tuple.template get<Is>()), ...);
            }

            [[nodiscard]] static constexpr auto get_size() noexcept {
                return size;
            }
        private:

            template<std::size_t... VIs>
            friend void print_impl(std::ostream& stream, const flat_tuple& tuple, std::index_sequence<VIs...>) {
                stream << "{ ";
                ((stream << (VIs == 0 ? "" : ", ") << tuple.template get<VIs>()), ...);
                stream << " };";
            }

            template<std::size_t N, typename NativeT>
            [[nodiscard]] constexpr const NativeT&
                get_impl() const noexcept {
                return static_cast<const NativeT&>(
                    static_cast<const indexed_value < NativeT, N >&> (*this).value
                    );
            }

            template<std::size_t N, typename NativeT>
            constexpr NativeT&
                get_impl() noexcept {
                return static_cast<NativeT&>(
                    static_cast<indexed_value < NativeT, N >&> (*this).value
                    );
            }

            template<std::size_t N>
            [[nodiscard]] constexpr auto deduce_type() const noexcept {
                // we cannot unambiguous determine holding type, thus try to cast to value and ref, and const ref...
                if constexpr (std::is_base_of_v<indexed_value<native_t<N>, N>, self_t>)
                    return type_holder<native_t<N>>{};
                else if constexpr (std::is_base_of_v<indexed_value<native_t<N>&, N>, self_t>)
                    return type_holder<native_t<N>&>{};
                else
                    return type_holder<const native_t<N>&>{};
            }

        };

        template <typename... Ts>
        using flat_tuple_t = flat_tuple_impl < std::make_index_sequence < size(type_list<Ts...>{}) > , Ts... > ;
    }

    template <typename... Ts>
    class flat_tuple final : public detail::flat_tuple_t<Ts...> {
    public:
        template <typename... VTs,
            typename = std::enable_if_t<std::is_same_v<type_list<std::decay_t<VTs>...>, type_list<std::decay_t<Ts>...>>>>
            constexpr flat_tuple(VTs&&... elems) noexcept
            : detail::flat_tuple_t<Ts...>(std::forward<VTs>(elems)...)
        { }
        constexpr flat_tuple()
            : detail::flat_tuple_t<Ts...>()
        { }
    };

    template <typename... Ts>
    flat_tuple(Ts...)->flat_tuple<Ts...>;

    template <typename... Ts>
    constexpr auto make_flat_tuple(Ts&&... args) {
        return flat_tuple<std::decay_t<Ts>...>(std::forward<Ts>(args)...);
    }

    template <typename... Ts>
    constexpr auto make_flat_tuple_bitfield_friendly(Ts... args) {
        return flat_tuple<std::decay_t<Ts>...>(std::forward<Ts>(args)...);
    }

    template <typename... Ts>
    constexpr auto make_flat_ref_tuple(Ts&&... args) {
        return flat_tuple<Ts...>(std::forward<Ts>(args)...);
    }

}