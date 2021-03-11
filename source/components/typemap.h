/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "typelist/type_list.h"

#include <type_traits>
#include <algorithm>
#include <array>

namespace hope {

    template <typename _Key, typename _Value>
    struct type_pair final {
        using Key = _Key;
        using Value = _Value;
    };

    template <typename... Types>
    class type_map final {
        constexpr static type_list<Types...> m_types;
    public:

        template <typename Key>
        static constexpr auto get() noexcept {
            constexpr auto index = index_of<Key>();
            static_assert(index < size(m_types));
            using pair = NthType<index, Types...>;
            return type_holder<pair::Value>();
        }

    private:

        template <typename T>
        struct type_extractor final {};

        template <typename K, typename V>
        struct type_extractor<type_pair<K, V>> final {
            using _Key = K;
            using _Value = V;
        };

        template <typename Key>
        constexpr static std::size_t index_of() {
            return find_if(m_types, [](auto pair) {
                using pair_t = decltype(pair);
                return std::is_same_v<Key, typename pair_t::Key>;
                });
        }

        template <typename K, typename V>
        constexpr static auto key(type_pair<K, V>) {
            return type_holder<K>{ };
        }

        template <typename T>
        struct type_checker final : public std::false_type {};

        template <typename Key, typename Value>
        struct type_checker<type_pair<Key, Value>> final : public std::true_type {};

        static_assert(all_of<type_checker>(m_types));
    };

}