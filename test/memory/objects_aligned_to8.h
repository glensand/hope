/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */
 
#pragma once

#include "hope/typelist/type_list.h"

using RegisteredTypesAscendingAlignedTo8 = hope::type_list
<
std::aligned_storage_t<8, 8>, 
std::aligned_storage_t<16, 8>, 
std::aligned_storage_t<24, 8>, 
std::aligned_storage_t<32, 8>, 
std::aligned_storage_t<40, 8>, 
std::aligned_storage_t<48, 8>, 
std::aligned_storage_t<56, 8>
>;

using RegisteredTypesDescendingAlignedTo8 = hope::type_list
<
std::aligned_storage_t<56, 8>, 
std::aligned_storage_t<48, 8>, 
std::aligned_storage_t<40, 8>, 
std::aligned_storage_t<32, 8>, 
std::aligned_storage_t<24, 8>, 
std::aligned_storage_t<16, 8>, 
std::aligned_storage_t<8, 8>
>;

using RegisteredTypesShuffledAlignedTo8 = hope::type_list
<
std::aligned_storage_t<40, 8>, 
std::aligned_storage_t<16, 8>, 
std::aligned_storage_t<48, 8>, 
std::aligned_storage_t<8, 8>, 
std::aligned_storage_t<32, 8>,  
std::aligned_storage_t<56, 8>, 
std::aligned_storage_t<24, 8> 
>;

