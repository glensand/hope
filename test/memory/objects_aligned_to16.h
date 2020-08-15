
/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */
 
#pragma once

#include "typelist/type_list.h"

using RegisteredTypesAscendingAlignedTo16 = hope::type_list
<
std::aligned_storage_t<16, 16>, 
std::aligned_storage_t<32, 16>, 
std::aligned_storage_t<48, 16>, 
std::aligned_storage_t<64, 16> 
>;

using RegisteredTypesDescendingAlignedTo16 = hope::type_list
<
std::aligned_storage_t<64, 16>, 
std::aligned_storage_t<48, 16>, 
std::aligned_storage_t<32, 16>, 
std::aligned_storage_t<16, 16> 
>;

using RegisteredTypesShuffledAlignedTo16 = hope::type_list
<
std::aligned_storage_t<32, 16>, 
std::aligned_storage_t<64, 16>, 
std::aligned_storage_t<48, 16>, 
std::aligned_storage_t<16, 16> 
>;

