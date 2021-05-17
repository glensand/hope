/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/typelist/type_list.h"
#include "hope/components/typemap.h"
#include "hope/components/policy_map/variant_list.h"

namespace hope::policy {

    /**
     * \brief Helper class which might be used to easy check if whether policy was set or not; When class instantiates
     * with policies, all of those will be checked to compatibility
     * \tparam VariantList List with variants of the policies; policy from one variant instance is not compatible and must not be used
     * together
     * \tparam PolicyBaseClass Base class to policies, all given Ps have to be derived (for security reasons only)
     * \tparam Ps pack with ps to be applied
     */
    template <
        typename VariantMap,
        typename PolicyBaseClass,
        typename... Ps
    >
    class map final {
    public:
        map() = delete;

        /**
         * \brief Check if specified candidate was set
         * \tparam Candidate type to be checked
         * \return true of successful check
         */
        template<typename Candidate>
        [[nodiscard]] constexpr static bool has() noexcept {
            constexpr type_list<Ps...> policies;
            return contains<Candidate>(policies);
        }

    private:
        template <typename... Ts>
        [[nodiscard]] static constexpr bool is_variant_list(variant_list<Ts...>) noexcept {
            return true;
        }

        [[nodiscard]] static constexpr bool is_variant_list(...) noexcept {
            return false;
        }

        static_assert(is_variant_list(VariantList{}));
        static_assert((... && std::is_base_of_v<PolicyBaseClass, Ps>));
    };

    template <
        typename VariantList,
        typename PolicyBaseClass,
        typename... Ps
    >
    struct get final {
        
    };
}
