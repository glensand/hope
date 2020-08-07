//------------------------------------------------------------------------------
// TupleFromStruct.h
// 
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 15.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

namespace hope {
    namespace detail {

        template <std::size_t I>
        struct any_convertible {
            std::size_t fake;
            template <typename T>
            constexpr operator T& () const noexcept;
        };

        template <typename T, std::size_t... Is>
        constexpr auto is_constructable_n(const T&, std::index_sequence<Is...>)
            ->decltype(T{ any_convertible<Is>{}... }, bool()) {
            return true;
        }

        template <typename T, std::size_t... Is>
        constexpr auto is_constructable_n(const T&, ...) {
            return false;
        }

        template <typename T, std::size_t... Is>
        constexpr std::size_t detect_fields_count_impl(const T& object, std::index_sequence<Is...> sequence) {
            bool bs[] = { is_constructable_n(object, std::make_index_sequence<Is>())... };
            // since cxx20... msvc sucks
            //return std::distance(std::begin(bs), std::find(
            //    std::begin(bs), std::end(bs), false)
            //);

            for (std::size_t i{ 1 }; i < size(sequence); ++i) {
                if (!bs[i]) return i - 1;
            }
            return 0;
        }
    }

    template <typename T>
    constexpr std::size_t detect_fields_count(const T& object) {
        constexpr auto size = sizeof(T);
        return detail::detect_fields_count_impl(object, std::make_index_sequence<size>());
    }
}
