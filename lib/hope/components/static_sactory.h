/* Copyright (C) 2021 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <array>
#include <utility>

namespace hope {

    /**
     * \brief Compile-time implementation of the Factory pattern. Integral types and enums can be used as Key; 
     */
    template <typename Interface,
              typename Key>
    class static_factory final {
        static_assert(std::is_integral_v<Key> || std::is_enum_v<Key>)
    public:
        factory() = delete;
        factory(const factory&) = delete;
        factory& operator=(const factory&) = delete;

        template <Key key, typename Type>
        constexpr static bool register_object() { 
            
        }

        [[nodiscard]] Interface* create(const Key& name) const {
            
            return nullptr;
        }

    private:
        //std::array<Interface*, ExpectedSize> m_map;
    };
}