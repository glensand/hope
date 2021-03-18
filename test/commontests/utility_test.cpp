/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "components/utility.h"

using namespace hope;

enum test_enum_1 : unsigned {
    val_0 = 0,
    val_1,
    val_2,
    val_3,
};

static_assert(any(val_0, val_1, val_2) == val_1);
static_assert(!(any(val_0, val_1, val_2) == val_3));

static_assert(!(all(val_0, val_1, val_2) == val_3));
static_assert(all(val_3, val_3, val_3) == val_3);
static_assert(all(val_0, val_1, val_2) != val_3);