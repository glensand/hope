/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "tuple/detect_fields_count.h"

struct ZeroFields {
    
};

static_assert(hope::detect_fields_count(ZeroFields{}) == 0);

struct OneField {
    int x;
};

static_assert(hope::detect_fields_count(OneField{}) == 1);

struct ThirdFields {
    int a, b, c;
};

static_assert(hope::detect_fields_count(ThirdFields{}) == 3);