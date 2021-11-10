/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/typelist/type_list.h"
#include <initializer_list>
#include <array>
#include <cassert>

namespace hope {

    template<typename TValue, typename... Ts>
    class type_value_map final {
    public:
        type_value_map(type_list<Ts...> list, std::initializer_list<TValue> values) {
            assert(size(list) == values.size() &&
                "hope::type_value_map:\n"
                "The size of the typelist and initializer list must be the same"
            );
            unsigned i{ 0 };
            for(auto&& v : values){
                m_values[i] = v;
                ++i;
            }
        }

    template<typename TDesiredType>
    decltype(auto) get() const {
        static_assert(contains<TDesiredType>(m_types), 
            "hope::type_value_map:\n"
            "Specified type is not present in the map");
        constexpr static auto index = find<TDesiredType>(m_types);
        return m_values[index];
    }

    private:
        constexpr static type_list<Ts...> m_types{ };
        std::array<TValue, size(m_types)> m_values{ };
    };

}