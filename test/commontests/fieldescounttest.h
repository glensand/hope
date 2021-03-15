/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "tuple/detect_fields_count.h"

struct fields_0 { };
static_assert(hope::detect_fields_count(fields_0{ }) == 0);

struct fields_1 { int x; };
static_assert(hope::detect_fields_count(fields_1{ }) == 1);

struct fields_3 { int a, b, c; };
static_assert(hope::detect_fields_count(fields_3{ }) == 3);

struct fields_4 { int a, b, c, d; };
static_assert(hope::detect_fields_count(fields_4{ }) == 4);