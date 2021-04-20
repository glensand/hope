/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <type_traits>
#include "hope/components/detector.h"

namespace hope {

    template <typename Child>
    struct any_convertible {

        template <typename T>
        using from_t = decltype(std::declval<T>().from(std::declval<const Child&>()));

        template <typename T>
        using assign_t = decltype(std::declval<T>().operator=(std::declval<const Child&>()));

        template <typename T>
        using static_from_t = decltype(T::from(std::declval<const Child&>()));

        template <typename T>
        constexpr operator T() {
            check_type<T>();
            if constexpr (is_detected<static_from_t, T>::value) {
                return T::from(child());
            }
            if constexpr (is_detected<from_t, T>::value){
                T res{ };
                res.from(child());
                return res;
            }
            if constexpr (is_detected<assign_t, T>::value) {
                T res{ };
                return res.operator=(child());
            }
            return T{ };
        }

    private:
        constexpr const Child& child() {
            return static_cast<const Child&>(*this);
        }

        template <typename T>
        constexpr static void check_type() {
            static_assert(std::is_convertible_v<Child, T>
                || is_detected<from_t, T>::value
                || is_detected<static_from_t,  T>::value
                || is_detected<assign_t, T>::value);
        }
    };
}