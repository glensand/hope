/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/typelist/type_list.h"
#include "hope/typelist/integraltypes.h"
#include <string>

namespace hope {

    constexpr static auto FixedAnyTypes = 
        concat(IntegralTypes, 
            type_list<
                std::string
            >{}
    );
}
