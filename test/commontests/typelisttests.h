#pragma once

#include "typelist/type_list.h"
#include <string>

static_assert(contains<int>(type_list<int, bool, float, double>{}));
static_assert(!contains<int>(type_list<bool, float, double>{}));

static_assert(Contains14<int>(type_list<int, bool, float, double>{}));
static_assert(!Contains14<int>(type_list<bool, float, double>{}));

static_assert(find<int>(type_list<int, bool, float, double>{}) == 0);
static_assert(find<unsigned>(type_list<int, bool, float, double>{}) == 4);

//static_assert(Find20<int>(type_list<int, bool, float, double>{}) == 0);
//static_assert(Find20<unsigned>(type_list<int, bool, float, double>{}) == 4);


static_assert(AllOf<std::is_pointer>(type_list<int*, bool*, float*, double*>{}));
static_assert(!AllOf<std::is_pointer>(type_list<int, bool, float, double>{}));
static_assert(AllOf<std::is_integral>(type_list<int, unsigned, long, long long, short>{}));
static_assert(AnyOf<std::is_pointer>(type_list<int, bool*, float*, double*>{}));


static_assert (LargestTypeIndex(type_list<std::string, double, float>{}) == 0);