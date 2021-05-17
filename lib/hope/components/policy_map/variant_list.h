/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/typelist/type_list.h"

namespace hope::policy {

    /**
     * \brief 
     * \tparam Variants 
     */
    template<typename... Variants>
    class variant final {
    public:
        constexpr variant() = default;

        /**
         * \brief 
         * \tparam Policies 
         * \return 
         */
        template <typename... Policies>
        [[nodiscard]] constexpr static bool check() noexcept {
            constexpr type_list<Variants...> variants;
            constexpr bool bs[] = { contains<Policies>(variants)... };
            std::size_t matches_count{ 0 };
            for (const auto& match : bs)
                if (match)
                    ++matches_count;
            return matches_count < 2; // if two mutually exclusive policies are specified, then the check is considered to be failed
        }
    };

    template<typename >

}
