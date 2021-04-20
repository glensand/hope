/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/typelist/type_list.h"

namespace hope {
    namespace detail {

        struct sort_helper {
            constexpr sort_helper(std::size_t index, std::size_t size) noexcept
                : type_index(index)
                , type_size(size)
            { }

            constexpr bool operator<(const sort_helper& r) const {
                return type_size < r.type_size;
            }

            constexpr bool operator>(const sort_helper& r) const {
                return type_size > r.type_size;
            }

            std::size_t     type_index;
            std::size_t     type_size;
        };

        template <typename T>
        constexpr void swap(T&& l, T&& r) {
            auto temp{ r };
            r = static_cast<T&&>(l);
            l = static_cast<T&&>(temp);
        }

        template<std::size_t... Is, typename... Ts>
        constexpr auto sort_impl(std::index_sequence<Is...> sequence, type_list<Ts...> list) {
            constexpr auto listSize = size(list);
            std::array<sort_helper, listSize> sizes{ sort_helper(Is, sizeof(Ts))... };

            // should be replaced with std::sort in cxx20
            for (size_t i{ 0 }; i < listSize; ++i) {
                auto it = std::max_element(std::begin(sizes) + i, std::end(sizes));
                size_t pos = std::distance(std::begin(sizes), it);
                if (i != pos)
                    swap(sizes[i], sizes[pos]);
            }
            return sizes;
        }

        template<std::size_t... Is, typename... Ts>
        constexpr auto sort_type_list(std::index_sequence<Is...> sequence, type_list<Ts...> list) {
            constexpr auto sorted_index_array = detail::sort_impl(sequence, list);
            return make_type_list(get_nth_type<sorted_index_array[Is].type_index>(list)...);
        }

    }

    template <typename... Ts>
    constexpr auto sort(type_list<Ts...> list) {
        constexpr auto sorted_index_array = detail::sort_type_list<>(std::make_index_sequence<size(list)>(), list);
        return sorted_index_array;
    }
}
