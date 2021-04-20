/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <type_traits>
#include <typeinfo>
#include <cstdint>
#include "hope/any/fixed_any_types.h"

namespace hope {

    namespace detail {

        struct fixed_any_holder_base{ };

        template <typename T>
        struct fixed_any_holder_impl final : fixed_any_holder_base {
            T value;
        };
    }

    class fixed_any final {
    public:
        template <typename T>
        explicit fixed_any(T&& val) {
            assert_type_registered<T>();
            if constexpr (is_small<T>()){
                build_small_pod(val);
            } else {
                build(val);
            }
            type_index = extract_type_index<T>();
        }

        template <typename T>
        const T& get() const {
            get_check<T>();
            if constexpr(is_small<T>())
                return reinterpret_cast<const T&>(holder);
            return static_cast<const detail::fixed_any_holder_impl<T>*>(holder)->value;
        }

        template <typename T>
        T& get() {
            get_check<T>();
            if constexpr (is_small<T>())
                return reinterpret_cast<T&>(holder);
            return static_cast<detail::fixed_any_holder_impl<T>*>(holder)->value;
        }

    private:

        template <typename T>
        constexpr static bool is_small() noexcept {
            return std::is_standard_layout_v<std::decay_t<T>>
                && sizeof(std::decay_t<T>) <= sizeof(detail::fixed_any_holder_base);
        }

        template <typename T>
        constexpr static uint8_t extract_type_index() noexcept  {
            return uint8_t(find<std::decay_t<T>>(FixedAnyTypes));
        }

        template <typename T>
        static void assert_type_registered() {
            static_assert(contains<std::decay_t<T>>(FixedAnyTypes));
        }

        template <typename T>
        void assert_containing_type() const {
            if (extract_type_index<T>() != type_index)
                throw std::bad_cast();
        }

        template <typename T>
        void get_check() const {
            assert_type_registered<T>();
            assert_containing_type<T>();
        }

        template <typename T>
        void build_small_pod(const T& val) {
            *reinterpret_cast<T*>(&holder) = val;
        }

        template <typename T>
        void build(const T& val) {
            *reinterpret_cast<T*>(&holder) = val;
        }

        detail::fixed_any_holder_base* holder;
        uint8_t type_index;
    };
}
