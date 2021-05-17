/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/components/policy_map/policy_map.h"

namespace hope::tuple_policy {

    /**
     * \brief base class to the tuple's policies
     */
    struct base{};

    /**
    * \brief Policy, used to specify how to convert struct to tuple. If instance of this structure is used,
    * all the fields from an object will be copied to the tuple
    */
    struct value final : base{};

    /**
    * \brief Policy, used to specify how to convert struct to tuple. If instance of this structure is used,
    * all the fields of resulting tuple will be references to the related fields of initial object(POD)
    */
    struct reference final : base{};

    /**
     * \brief
     */
    struct bit final : base{ };

    /**
     * \brief
     */
    struct byte final : base{ };

    /**
     * \brief
     */
    struct recursive final : base { };

    /**
    * \brief
    */
    struct non_recursive final : base { };

    template<typename... Ps>
    using tuple = policy::map<
        policy::variant_list<
            policy::variant<bit, byte>,
            policy::variant<bit, reference>,
            policy::variant<value, reference>,
            policy::variant<recursive, non_recursive>
        >,
        base,
        Ps...
    >;
}