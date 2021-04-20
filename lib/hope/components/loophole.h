/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

namespace hope::loophole {

    namespace detail {
        template <typename T, std::size_t I>
        struct def_inserter_i {
            friend constexpr auto get_impl(def_inserter_i<T, I>);
        };

        template <typename>
        struct def_inserter {
            friend constexpr auto get_impl(def_inserter);
        };

        template <typename K, typename V>
        struct decl_inserter {
            friend constexpr auto get_impl(K) {
                return V{};
            }
        };
    }
    
    template <typename K, typename V>
    constexpr void inject(K, V) {
        (void)sizeof(detail::decl_inserter<detail::def_inserter<K>, V>);
    }

    template <std::size_t I, typename K, typename V>
    constexpr void inject(K, V) {
        (void)sizeof(detail::decl_inserter<detail::def_inserter_i<K, I>, V>);
    }

    template <typename K>
    constexpr auto get(K) {
        return get_impl(detail::def_inserter<K>{});
    }

    template <std::size_t I, typename K>
    constexpr auto get(K) {
        return get_impl(detail::def_inserter_i<K, I>{});
    }

}
