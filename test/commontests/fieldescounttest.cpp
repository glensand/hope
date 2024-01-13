/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "hope/tuple/detect_fields_count.h"
#include "hope/tuple/compute_field_count_recursive.h"

struct fields_0 { };
static_assert(hope::detect_fields_count<fields_0>() == 0);
static_assert(hope::compute_field_count_recursive_constexpr<fields_0>() == 0);

template<typename T>
struct fields_1 { T x; };
static_assert(hope::detect_fields_count<fields_1<int>>() == 1);
static_assert(hope::detect_fields_count<fields_1<bool>>() == 1);

static_assert(hope::compute_field_count_recursive_constexpr<fields_1<int>>() == 1);
static_assert(hope::compute_field_count_recursive_constexpr<fields_1<bool>>() == 1);

template <typename T>
struct fields_3 { T a, b, c; };
static_assert(hope::detect_fields_count<fields_3<int>>() == 3);
static_assert(hope::detect_fields_count<fields_3<bool>>() == 3);

static_assert(hope::compute_field_count_recursive_constexpr<fields_3<int>>() == 3);
static_assert(hope::compute_field_count_recursive_constexpr<fields_3<bool>>() == 3);

template <typename T>
struct fields_4 { T a, b, c, d; };
static_assert(hope::detect_fields_count<fields_4<int>>() == 4);
static_assert(hope::detect_fields_count< fields_4<bool>>() == 4);

static_assert(hope::compute_field_count_recursive_constexpr<fields_4<int>>() == 4);
static_assert(hope::compute_field_count_recursive_constexpr< fields_4<bool>>() == 4);

struct struct_with_sting {
    std::string s1;
    std::string s2;
    std::string s3;
    bool b1 = true;
    float f1 = 1.f;
    int i1 = 0;
};

static_assert(hope::detect_fields_count<struct_with_sting>() == 6);

struct recursive_struct_string final {
    struct_with_sting s1;
    struct_with_sting s2;
    std::string s3;
};

static_assert(hope::detect_fields_count<recursive_struct_string>() == 3);

