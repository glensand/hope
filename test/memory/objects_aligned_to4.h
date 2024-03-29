/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */
 
#pragma once

#include "hope/typelist/type_list.h"

using RegisteredTypesAscendingAlignedTo4 = hope::type_list
<
std::aligned_storage_t<4, 4>, 
std::aligned_storage_t<8, 4>, 
std::aligned_storage_t<12, 4>, 
std::aligned_storage_t<16, 4>, 
std::aligned_storage_t<20, 4>, 
std::aligned_storage_t<24, 4>, 
std::aligned_storage_t<28, 4>, 
std::aligned_storage_t<32, 4>, 
std::aligned_storage_t<36, 4>, 
std::aligned_storage_t<40, 4>, 
std::aligned_storage_t<44, 4>, 
std::aligned_storage_t<48, 4>, 
std::aligned_storage_t<52, 4>, 
std::aligned_storage_t<56, 4>
>;

using RegisteredTypesDescendingAlignedTo4 = hope::type_list
<
std::aligned_storage_t<56, 4>, 
std::aligned_storage_t<52, 4>, 
std::aligned_storage_t<48, 4>, 
std::aligned_storage_t<44, 4>, 
std::aligned_storage_t<40, 4>, 
std::aligned_storage_t<36, 4>, 
std::aligned_storage_t<32, 4>, 
std::aligned_storage_t<28, 4>, 
std::aligned_storage_t<24, 4>, 
std::aligned_storage_t<20, 4>, 
std::aligned_storage_t<16, 4>, 
std::aligned_storage_t<12, 4>, 
std::aligned_storage_t<8, 4>,
std::aligned_storage_t<4, 4>
>;

using RegisteredTypesShuffledAlignedTo4 = hope::type_list
<
std::aligned_storage_t<16, 4>,
std::aligned_storage_t<56, 4>,
std::aligned_storage_t<52, 4>,
std::aligned_storage_t<48, 4>,
std::aligned_storage_t<12, 4>,
std::aligned_storage_t<44, 4>,
std::aligned_storage_t<20, 4>,
std::aligned_storage_t<4, 4>,
std::aligned_storage_t<40, 4>,
std::aligned_storage_t<24, 4>,
std::aligned_storage_t<28, 4>,
std::aligned_storage_t<8, 4>,
std::aligned_storage_t<36, 4>,
std::aligned_storage_t<32, 4>
>;

    