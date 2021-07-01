/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <utility>

namespace hope {
    namespace detail {

        template <typename TStruct, std::size_t I>
        struct any_convertible {

            template <typename T, typename = std::enable_if_t<!std::is_base_of_v<T, TStruct>>>
            constexpr operator T& () const noexcept;
        };

        template <typename T, std::size_t... Is>
        constexpr auto is_constructable_n(std::index_sequence<Is...>)
            ->decltype(T{ any_convertible<T, Is>{}... }, bool()) {
            return true;
        }

        template <typename T, std::size_t... Is>
        constexpr auto is_constructable_n(...) {
            return false;
        }

        template <typename T, std::size_t... Is>
        constexpr std::size_t detect_fields_count_impl(std::index_sequence<Is...> sequence) {
            bool bs[] = { is_constructable_n<T>(std::make_index_sequence<Is>())... };
            // since cxx20... msvc sucks
            //return std::distance(std::begin(bs), std::find(
            //    std::begin(bs), std::end(bs), false)
            //);

            for (std::size_t i{ 1 }; i < sequence.size(); ++i) {
                if (!bs[i]) return i - 1;
            }
            return 0;
        }
    }

    template <typename T>
    constexpr std::size_t detect_fields_count() {

        static_assert(!std::is_polymorphic_v<T>, "---- hope ---- Polymorphic types are not allowed");
        static_assert(std::is_aggregate_v<T>, "---- hope ---- Only aggregate initializable types are allowed");

        constexpr auto size = sizeof(T);
        return detail::detect_fields_count_impl<T>(std::make_index_sequence<size + 2>());
    }
}
