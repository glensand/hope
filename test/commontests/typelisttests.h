/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "typelist/type_list.h"
#include <string>

static_assert(contains<int>(hope::type_list<int, bool, float, double>{}));
static_assert(!contains<int>(hope::type_list<bool, float, double>{}));

static_assert(contains14<int>(hope::type_list<int, bool, float, double>{}));
static_assert(!contains14<int>(hope::type_list<bool, float, double>{}));

static_assert(find<int>(hope::type_list<int, bool, float, double>{}) == 0);
static_assert(find<unsigned>(hope::type_list<int, bool, float, double>{}) == 4);

//static_assert(Find20<int>(type_list<int, bool, float, double>{}) == 0);
//static_assert(Find20<unsigned>(type_list<int, bool, float, double>{}) == 4);


static_assert(hope::all_of<std::is_pointer>(hope::type_list<int*, bool*, float*, double*>{}));
static_assert(!hope::all_of<std::is_pointer>(hope::type_list<int, bool, float, double>{}));
static_assert(hope::all_of<std::is_integral>(hope::type_list<int, unsigned, long, long long, short>{}));
static_assert(hope::any_of<std::is_pointer>(hope::type_list<int, bool*, float*, double*>{}));


static_assert (largest_type_index(hope::type_list<std::string, double, float>{}) == 0);