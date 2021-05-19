/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

namespace hope {

    /**
     * \brief struct used as namespace to policies which is used for tuple creation functions
     */
    struct field_policy final {
        /**
         * \brief Policy, used to specify how to convert struct to tuple. If instance of this structure is used,
         * all the fields from an object will be copied to the tuple
         */
        struct value final {};

        /**
         * \brief Policy, used to specify how to convert struct to tuple. If instance of this structure is used,
         * all the fields of resulting tuple will be references to the related fields of initial object(POD)
         */
        struct reference final {};
    };

}