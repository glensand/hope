/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/typelist/type_list.h"
#include "hope/foundation.h"

/*! \defgroup <reflection> Static reflection
    @{
    \file
    \brief Implementation of non - recursive tuple, tuple size and alignment is same as structure with fields of
    specified types; Also this class contains a lot of useful methods.
*/

namespace hope {
    namespace detail {

        /**
         * \brief Tag is used to identify whether object of provided type is tuple or not
         */
        struct tuple_tag {
        };

        /**
         * \breaf Structure used to create non recursive tuple via inheritance.
         * @tparam T Corresponding type, value of this type is stored as single field
         * @tparam I Index of current field (transparent). Is used to unambiguously determine value
         */
        template<typename T, size_t I>
        struct indexed_value {
            constexpr indexed_value() = default;

            constexpr indexed_value(indexed_value &&) = default;

            constexpr indexed_value(const indexed_value &) = default;

            ~indexed_value() = default;

            template<typename Vt>
            explicit constexpr indexed_value(Vt &&valueRef)
                    : value(std::forward<Vt>(valueRef)) {
                static_assert(std::is_constructible_v<T, Vt>,
                              "hope::indexed_value: given object is not suitable as initializer"
                );
            }

            T value;
        };

        template<typename, typename... Ts>
        class flat_tuple_impl;

        /**
         * \breaf General class of the all static reflection module.
         * @tparam Is Helper sequence of indexes, max(Is...) + is the count of the tuple's elements
         * @tparam Ts Types stored in the tuple
         */
        template<std::size_t... Is, typename... Ts>
        class flat_tuple_impl<std::index_sequence<Is...>, Ts...> : public tuple_tag, public indexed_value<Ts, Is> ... {
            using self_t = flat_tuple_impl<std::index_sequence<Is...>, Ts...>;
            constexpr static type_list<std::decay_t<Ts>...> types{}; // only clear types might be used, thus make it clear here

            template<std::size_t I>
            using native_t = std::decay_t<typename decltype(get_nth_type<I>(types))::Type>;
        protected:
            using flat_tuple = flat_tuple_impl<std::index_sequence<Is...>, Ts...>;

            constexpr flat_tuple_impl() = default;
            constexpr flat_tuple_impl(const flat_tuple_impl &) = default;
            constexpr flat_tuple_impl(flat_tuple_impl &&) = default;

            ~flat_tuple_impl() = default;

            template<typename... VTs>
            constexpr flat_tuple_impl(VTs &&... elems) noexcept
                    : indexed_value<Ts, Is>(std::forward<VTs>(elems))... {}

        public:
            constexpr static std::size_t tuple_size{size(types)};

            /**
             * \brief Tries to find element of specified type, fails on static assert if element had not been found; method is not sensitive to the
             * qualifiers, followed constructions are equal: get<int>() get<int&>() , get<const int>(), get<const int&>(); accessed value will be automatically deduced
             * by tuple's type (regardless of field policy is being used to create the tuple).
             *
             * \tparam T : Type of element to be returned, const and reference qualifiers does not matter
             *
             * \tparam NativeT : clear type, used to deduce return type and check whether tuple has element with specified value or not 
             *
             * \return reference to the containing element
             */
            template<typename T, typename NativeT = std::decay_t<T>>
            [[nodiscard]] constexpr decltype(auto)
            get() noexcept {
                static_assert(contains<NativeT>(types));
                constexpr auto TypeIndex = find<NativeT>(types);
                using value_t = typename decltype(deduce_type<NativeT, TypeIndex>())::Type;
                return get_impl<TypeIndex, value_t>();
            }

            /**
            * \brief Tries to find element of specified type, fails on static assert if element had not been found; method is not sensitive to the
            * qualifiers, followed constructions are equal: get<int>() get<int&>() , get<const int>(), get<const int&>(); accessed value will be automatically deduced
            * by tuple's type (regardless of field policy is being used to create the tuple).
            *
            * \tparam T : Type of element to be returned, const and reference qualifiers does not matter
            *
            * \tparam NativeT : clear type, used to deduce return type and check whether tuple has element with specified value or not
            *
            * \return reference to the containing element
            */
            template<typename T, typename NativeT = std::decay_t<T>>
            [[nodiscard]] constexpr decltype(auto)
            get() const noexcept {
                static_assert(contains<NativeT>(types));
                constexpr auto TypeIndex = find<NativeT>(types);
                using value_t = typename decltype(deduce_type<NativeT, TypeIndex>())::Type;
                return get_impl<TypeIndex, value_t>();
            }

            /**
             * \brief Tries to find element with given index
             * \tparam N Index of the element to be found
             * \return Reference to the containing element, const and ref qualifiers of the containing one are not change,
             * ref will be returned "as is"
             */
            template<size_t N>
            [[nodiscard]] constexpr decltype(auto)
            get() noexcept {
                static_assert(tuple_size > N);
                using nth_t = native_t<N>;
                using value_t = typename decltype(deduce_type<nth_t, N>())::Type;
                return get_impl<N, value_t>();
            }

            /**
             * \brief Tries to find element with given index
             * \tparam N Index of the element to be found
             * \return Reference to the containing element, const and ref qualifiers of the containing one do not change,
             * ref will be returned "as is"
             */
            template<size_t N>
            [[nodiscard]] constexpr decltype(auto)
            get() const noexcept {
                static_assert(tuple_size > N);
                using nth_t = native_t<N>;
                using value_t = typename decltype(deduce_type<nth_t, N>())::Type;
                return get_impl<N, value_t>();
            }

            /**
             * \brief Applies given functor to each value of tuple, more useful analogue of std::apply 
             * \tparam F type of functional object 
             * \param f functional object to be sequentially applied to each field of tuple
             */
            template<typename F>
            constexpr void for_each(F &&f) const {
                (f(get<Is>()), ...);
            }

            /**
            * \brief Applies given functor to each value of tuples
            * \tparam F type of functional object
            * \param tuple2 second tuple the function to be applied to
            * \param f functional object to be sequentially applied to each field of tuple
            */
            template<typename F>
            constexpr void for_each(const flat_tuple &tuple2, F &&f) const {
                (f(get<Is>(), tuple2.template get<Is>()), ...);
            }

            /**
             * \brief Applies given functor to each value of given tuple, more useful analogue of std::apply
             * \tparam F type of functional object
             * \param f functional object to be sequentially applied to each field of tuple
             */
            template<typename F>
            constexpr void for_each(F &&f) {
                (f(get<Is>()), ...);
            }

            /**
            * \brief Applies given functor to each value of tuples
            * \tparam F type of functional object
            * \param tuple2 second tuple the function to be applied to
            * \param f functional object to be sequentially applied to each field of tuple
            */
            template<typename F>
            constexpr void for_each(flat_tuple &tuple2, F &&f) {
                (f(get<Is>(), tuple2.template get<Is>()), ...);
            }

            /**
             * \brief just returns the size of the tuple (means number of fields)
             * \return number of fields
             */
            [[nodiscard]] static constexpr auto get_size() noexcept {
                return tuple_size;
            }

        private:

            /**
             * \brief Cast self to the specified type (indexed value instantiated with N and NativeT) 
             * \tparam N index of value to be returned
             * \tparam NativeT final type of value to be returned
             * \return reference of the containing value
             */
            template<std::size_t N, typename NativeT>
            [[nodiscard]] constexpr decltype(auto)
            get_impl() const noexcept {
                return static_cast<const NativeT &>(
                        static_cast<const indexed_value<NativeT, N> &> (*this).value
                );
            }

            /**
             * \brief Cast self to the specified type (indexed value instantiated with N and NativeT)
             * \tparam N index of value to be returned
             * \tparam NativeT final type of value to be returned
             * \return reference of the containing value
             */
            template<std::size_t N, typename NativeT>
            constexpr decltype(auto)
            get_impl() noexcept {
                return static_cast<NativeT &>(
                        static_cast<indexed_value<NativeT, N> &> (*this).value
                );
            }

            /**
             * \brief Tries to deduce final type of the value with specified type and given index
             * \tparam T : clear type to be used to deduce final type 
             * \tparam I : index of element to be processed
             * \return : final type of the first value which decayed type is match with given one
             */
            template<typename T, std::size_t I>
            [[nodiscard]] constexpr auto deduce_type() const noexcept {
                // we cannot unambiguous determine holding type, thus try to cast to value and ref, and const ref...
                if constexpr (std::is_base_of_v<::hope::detail::indexed_value<T, I>, self_t>) //fixed crazy clion error
                    return type_holder<T>{};
                else if constexpr (std::is_base_of_v<::hope::detail::indexed_value<T &, I>, self_t>) //fixed crazy clion error
                    return type_holder<T &>{};
                else
                    return type_holder<const T &>{};
            }
        };

        template<typename... Ts>
        using flat_tuple_t = flat_tuple_impl<std::make_index_sequence<size(type_list<Ts...>{})>, Ts...>;
    }

    /**
     * \brief Implementation of the struct like non recursive tuple which might be used to directly cast to struct with same fields
     * because alignment of the structs's fields and tuple's fields are match 
     * \tparam Ts types which might be stored in the tuple
     */
    template<typename... Ts>
    class flat_tuple final : public detail::flat_tuple_t<Ts...> {
    public:
        template<typename... VTs,
                typename = std::enable_if_t<std::is_same_v<type_list<std::decay_t<VTs>...>, type_list<std::decay_t<Ts>...>>>>
        constexpr flat_tuple(VTs &&... elems) noexcept
                : detail::flat_tuple_t<Ts...>(std::forward<VTs>(elems)...) {}

        constexpr flat_tuple()
                : detail::flat_tuple_t<Ts...>() {}
    };

    template<typename... Ts>
    flat_tuple(Ts...)->flat_tuple<Ts...>;

    /**
     * \brief Helper function, which should be used to create flat tuple with given arguments
     * all given values will be forwarded to the tuple instance
     * \tparam Ts type pack to be propagate to the flat tuple
     * \param args arguments are used to initialize each field of the tuple
     * \return created tuple
     */
    template<typename... Ts>
    constexpr auto make_flat_tuple(Ts &&... args) {
        return flat_tuple<std::decay_t<Ts>...>(std::forward<Ts>(args)...);
    }

    /**
     * \brief Helper function, should be used to create tuple of elements with specified bit size
     * \tparam Ts type pack to be propagate to the flat tuple
     * \param args arguments are used to initialize each field of the tuple
     * \return created tuple
     */
    template<typename... Ts>
    constexpr auto make_flat_tuple_bitfield_friendly(Ts... args) {
        return flat_tuple<std::decay_t<Ts>...>(std::forward<Ts>(args)...);
    }

    /**
     * \brief Helper function, creates tuple with references to the given arguments; const quality of arguments do not changes
     * \tparam Ts type pack to be propagate to the flat tuple
     * \param args arguments to create references for
     * \return tuple of references to given objects
     */
    template<typename... Ts>
    constexpr auto make_flat_ref_tuple(Ts &&... args) {
        return flat_tuple<Ts...>(std::forward<Ts>(args)...);
    }

    /**
     * Helper function, should be used to call corresponding function on each value of the provided tuple.
     * SFINAE is used to disable this function to other types, which might be used in the functions with the
     * same signature, such as type_list::for_each
     * @tparam Tuple Type of the tuple the function to be applied to, only hope::flat_tuple is allowed
     * @tparam F The type of the function
     * @param tuple Instance of the tuple object(const ref and ref might be used)
     * @param f Instance of the functor to be applied to
     */
    template<typename Tuple, typename F>
    constexpr std::enable_if_t<std::is_base_of_v<detail::tuple_tag, std::decay_t<Tuple>>>
    for_each(Tuple &&tuple, F &&f) {
        tuple.for_each(std::forward<F>(f));
    }

    /**
     * Helper function, should be used to call corresponding function on each value of the provided tuples.
     * SFINAE is used to disable this function to other types, which might be used in the functions with the
     * same signature, such as type_list::for_each
     * @tparam Tuple Type of the tuple the function to be applied to, only hope::flat_tuple is allowed
     * @tparam F The type of the function
     * @param tuple1 Instance of the first tuple object(const ref and ref might be used). Values from this object will be
     * given as first argument to the desired function.
     * @param tuple2 Same as tuple1, but values of this tuple will be provided to the desired function as second arg.
     * @param f The instance of the function to be called
     */
    template<typename Tuple, typename F>
    constexpr std::enable_if_t<std::is_base_of_v<detail::tuple_tag, std::decay_t<Tuple>>>
    for_each(Tuple &&tuple1, Tuple &&tuple2, F &&f) {
        tuple1.for_each(std::forward<Tuple>(tuple2), std::forward<F>(f));
    }
}

/*! @} */