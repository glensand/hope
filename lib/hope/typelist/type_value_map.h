/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/typelist/type_list.h"
#include "hope/tuple/flat_tuple.h"
#include <initializer_list>
#include <array>
#include <cassert>

namespace hope {

    template<typename TKey, typename TValue>
    struct type_value final {
        using key_t = TKey;
        using value_t = TValue;

        TValue value;
    };

    template<typename TKey, typename TValue>
    auto tv(const TValue& v){
        return type_value<TKey, TValue>{ v };
    }

    template<typename... Ts>
    class type_value_map final {
        template<typename T>
        struct is_type_value final : std::false_type{};

        template<typename TKey, typename TValue>
        struct is_type_value<type_value<TKey, TValue>> final : std::true_type{};

        template<typename T>
        constexpr static auto is_type_value_v = is_type_value<T>::value;

    public:
        type_value_map(const Ts&... vs) 
            : m_map(vs...) {
            m_map.for_each([](auto&& pair){
                using pair_t = std::decay_t<decltype(pair)>;
                static_assert(is_type_value_v<pair_t>, 
                    "HOPE STATIC ASSRTATION FAILED:\n"
                    "Onve of the arguments is not an instance of the hope::type_value structure.\n"
                    "hope::type_value_map is intended to work only with hope::type_value."
                    "Wrap your key and value to it and try to compile again."
                );
            });
        }

        template<typename TKey>
        decltype(auto) get() const {
            constexpr static auto idx = index<TKey>();
            static_assert(idx < size(type_list<Ts...>{}), 
                "HOPE STATIC ASSRTATION FAILED:\n"
                "Given type is not present in the specified type - value map"
            );

            return m_map.template get<idx>().value;
        }

        template<typename TKey, typename TValue>
        void set(TValue&& val){
            using pair_t = type_value<TKey, TValue>;
            static_assert(contains<pair_t>(type_list<Ts...>{}), 
                "HOPE STATIC ASSRTATION FAILED:\n"
                "Given combination of key type and value type is not present in type value map"
            );
            m_map.template get<pair_t>().value = val;
        }

    private:

        template<typename TKey>
        constexpr static std::size_t index(){
            return find_if(type_list<Ts...>{}, [] (auto pair) {
                using pair_t = std::decay_t<decltype(pair)>;
                using key_t = typename pair_t::Type::key_t;
                return std::is_same_v<TKey, key_t>;
            });
        }
        const hope::flat_tuple<Ts...> m_map;
    };

    template<typename... Ts>
    type_value_map(Ts...)->type_value_map<std::decay_t<Ts>...>;
}