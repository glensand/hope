/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */
 
#pragma once

#include "hope/typelist/type_list.h"

using RegisteredTypesAscendingAlignedTo1 = hope::type_list
<
std::aligned_storage_t<1, 1>, 
std::aligned_storage_t<2, 1>, 
std::aligned_storage_t<3, 1>, 
std::aligned_storage_t<4, 1>, 
std::aligned_storage_t<5, 1>, 
std::aligned_storage_t<6, 1>, 
std::aligned_storage_t<7, 1>, 
std::aligned_storage_t<8, 1>, 
std::aligned_storage_t<9, 1>, 
std::aligned_storage_t<10, 1>, 
std::aligned_storage_t<11, 1>, 
std::aligned_storage_t<12, 1>, 
std::aligned_storage_t<13, 1>, 
std::aligned_storage_t<14, 1>, 
std::aligned_storage_t<15, 1>, 
std::aligned_storage_t<16, 1>, 
std::aligned_storage_t<17, 1>, 
std::aligned_storage_t<18, 1>, 
std::aligned_storage_t<19, 1>, 
std::aligned_storage_t<20, 1>, 
std::aligned_storage_t<21, 1>, 
std::aligned_storage_t<22, 1>, 
std::aligned_storage_t<23, 1>, 
std::aligned_storage_t<24, 1>, 
std::aligned_storage_t<25, 1>, 
std::aligned_storage_t<26, 1>, 
std::aligned_storage_t<27, 1>, 
std::aligned_storage_t<28, 1>, 
std::aligned_storage_t<29, 1>, 
std::aligned_storage_t<30, 1>, 
std::aligned_storage_t<31, 1>, 
std::aligned_storage_t<32, 1>, 
std::aligned_storage_t<33, 1>, 
std::aligned_storage_t<34, 1>, 
std::aligned_storage_t<35, 1>, 
std::aligned_storage_t<36, 1>, 
std::aligned_storage_t<37, 1>, 
std::aligned_storage_t<38, 1>, 
std::aligned_storage_t<39, 1>, 
std::aligned_storage_t<40, 1>, 
std::aligned_storage_t<41, 1>, 
std::aligned_storage_t<42, 1>, 
std::aligned_storage_t<43, 1>, 
std::aligned_storage_t<44, 1>, 
std::aligned_storage_t<45, 1>, 
std::aligned_storage_t<46, 1>, 
std::aligned_storage_t<47, 1>, 
std::aligned_storage_t<48, 1>, 
std::aligned_storage_t<49, 1>, 
std::aligned_storage_t<50, 1>, 
std::aligned_storage_t<51, 1>, 
std::aligned_storage_t<52, 1>, 
std::aligned_storage_t<53, 1>, 
std::aligned_storage_t<54, 1>, 
std::aligned_storage_t<55, 1>, 
std::aligned_storage_t<56, 1>, 
std::aligned_storage_t<57, 1>, 
std::aligned_storage_t<58, 1>, 
std::aligned_storage_t<59, 1>, 
std::aligned_storage_t<60, 1>
>;

using RegisteredTypesDescendingAlignedTo1 = hope::type_list
< 
std::aligned_storage_t<60, 1>, 
std::aligned_storage_t<59, 1>, 
std::aligned_storage_t<58, 1>, 
std::aligned_storage_t<57, 1>, 
std::aligned_storage_t<56, 1>, 
std::aligned_storage_t<55, 1>, 
std::aligned_storage_t<54, 1>, 
std::aligned_storage_t<53, 1>, 
std::aligned_storage_t<52, 1>, 
std::aligned_storage_t<51, 1>, 
std::aligned_storage_t<50, 1>, 
std::aligned_storage_t<49, 1>, 
std::aligned_storage_t<48, 1>, 
std::aligned_storage_t<47, 1>, 
std::aligned_storage_t<46, 1>, 
std::aligned_storage_t<45, 1>, 
std::aligned_storage_t<44, 1>, 
std::aligned_storage_t<43, 1>, 
std::aligned_storage_t<42, 1>, 
std::aligned_storage_t<41, 1>, 
std::aligned_storage_t<40, 1>, 
std::aligned_storage_t<39, 1>, 
std::aligned_storage_t<38, 1>, 
std::aligned_storage_t<37, 1>, 
std::aligned_storage_t<36, 1>, 
std::aligned_storage_t<35, 1>, 
std::aligned_storage_t<34, 1>, 
std::aligned_storage_t<33, 1>, 
std::aligned_storage_t<32, 1>, 
std::aligned_storage_t<31, 1>, 
std::aligned_storage_t<30, 1>, 
std::aligned_storage_t<29, 1>, 
std::aligned_storage_t<28, 1>, 
std::aligned_storage_t<27, 1>, 
std::aligned_storage_t<26, 1>, 
std::aligned_storage_t<25, 1>, 
std::aligned_storage_t<24, 1>, 
std::aligned_storage_t<23, 1>, 
std::aligned_storage_t<22, 1>, 
std::aligned_storage_t<21, 1>, 
std::aligned_storage_t<20, 1>, 
std::aligned_storage_t<19, 1>, 
std::aligned_storage_t<18, 1>, 
std::aligned_storage_t<17, 1>, 
std::aligned_storage_t<16, 1>, 
std::aligned_storage_t<15, 1>, 
std::aligned_storage_t<14, 1>, 
std::aligned_storage_t<13, 1>, 
std::aligned_storage_t<12, 1>, 
std::aligned_storage_t<11, 1>, 
std::aligned_storage_t<10, 1>, 
std::aligned_storage_t<9, 1>, 
std::aligned_storage_t<8, 1>, 
std::aligned_storage_t<7, 1>, 
std::aligned_storage_t<6, 1>, 
std::aligned_storage_t<5, 1>, 
std::aligned_storage_t<4, 1>, 
std::aligned_storage_t<3, 1>, 
std::aligned_storage_t<2, 1>, 
std::aligned_storage_t<1, 1>
>;

using RegisteredTypesShuffledAlignedTo1 = hope::type_list
<
std::aligned_storage_t<11, 1>, 
std::aligned_storage_t<60, 1>, 
std::aligned_storage_t<10, 1>, 
std::aligned_storage_t<14, 1>, 
std::aligned_storage_t<15, 1>, 
std::aligned_storage_t<36, 1>, 
std::aligned_storage_t<21, 1>, 
std::aligned_storage_t<59, 1>, 
std::aligned_storage_t<41, 1>, 
std::aligned_storage_t<40, 1>, 
std::aligned_storage_t<23, 1>, 
std::aligned_storage_t<8, 1>, 
std::aligned_storage_t<51, 1>, 
std::aligned_storage_t<17, 1>, 
std::aligned_storage_t<39, 1>, 
std::aligned_storage_t<47, 1>, 
std::aligned_storage_t<31, 1>, 
std::aligned_storage_t<35, 1>, 
std::aligned_storage_t<38, 1>, 
std::aligned_storage_t<28, 1>, 
std::aligned_storage_t<53, 1>, 
std::aligned_storage_t<44, 1>, 
std::aligned_storage_t<6, 1>, 
std::aligned_storage_t<9, 1>, 
std::aligned_storage_t<26, 1>, 
std::aligned_storage_t<13, 1>, 
std::aligned_storage_t<49, 1>, 
std::aligned_storage_t<58, 1>, 
std::aligned_storage_t<19, 1>, 
std::aligned_storage_t<5, 1>, 
std::aligned_storage_t<7, 1>, 
std::aligned_storage_t<30, 1>, 
std::aligned_storage_t<25, 1>, 
std::aligned_storage_t<57, 1>, 
std::aligned_storage_t<43, 1>, 
std::aligned_storage_t<18, 1>, 
std::aligned_storage_t<54, 1>,  
std::aligned_storage_t<34, 1>, 
std::aligned_storage_t<1, 1>, 
std::aligned_storage_t<46, 1>, 
std::aligned_storage_t<37, 1>, 
std::aligned_storage_t<48, 1>, 
std::aligned_storage_t<2, 1>, 
std::aligned_storage_t<16, 1>, 
std::aligned_storage_t<4, 1>, 
std::aligned_storage_t<45, 1>, 
std::aligned_storage_t<24, 1>, 
std::aligned_storage_t<42, 1>, 
std::aligned_storage_t<22, 1>, 
std::aligned_storage_t<52, 1>, 
std::aligned_storage_t<33, 1>, 
std::aligned_storage_t<3, 1>, 
std::aligned_storage_t<32, 1>, 
std::aligned_storage_t<12, 1>, 
std::aligned_storage_t<55, 1>, 
std::aligned_storage_t<56, 1>, 
std::aligned_storage_t<27, 1>, 
std::aligned_storage_t<20, 1>, 
std::aligned_storage_t<29, 1>, 
std::aligned_storage_t<50, 1>
>;

