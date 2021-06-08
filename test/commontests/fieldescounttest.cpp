/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "hope/tuple/detect_fields_count.h"
#include "hope/tuple/tuple_policy.h"

struct fields_0 { };
static_assert(hope::detect_fields_count<fields_0>() == 0);

template<typename T>
struct fields_1 { T x; };
static_assert(hope::detect_fields_count<fields_1<int>>() == 1);
static_assert(hope::detect_fields_count<fields_1<bool>>() == 1);

template <typename T>
struct fields_3 { T a, b, c; };
static_assert(hope::detect_fields_count<fields_3<int>>() == 3);
static_assert(hope::detect_fields_count<fields_3<bool>>() == 3);

template <typename T>
struct fields_4 { T a, b, c, d; };
static_assert(hope::detect_fields_count<fields_4<int>>() == 4);
static_assert(hope::detect_fields_count< fields_4<bool>>() == 4);