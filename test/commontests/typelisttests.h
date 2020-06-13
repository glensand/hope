#pragma once

#include "typelist/TypeList.h"
#include <string>

static_assert(Contains<int>(TypeList<int, bool, float, double>{}));
static_assert(!Contains<int>(TypeList<bool, float, double>{}));

static_assert(Contains14<int>(TypeList<int, bool, float, double>{}));
static_assert(!Contains14<int>(TypeList<bool, float, double>{}));

static_assert(Find<int>(TypeList<int, bool, float, double>{}) == 0);
static_assert(Find<unsigned>(TypeList<int, bool, float, double>{}) == 4);

//static_assert(Find20<int>(TypeList<int, bool, float, double>{}) == 0);
//static_assert(Find20<unsigned>(TypeList<int, bool, float, double>{}) == 4);


static_assert(AllOf<std::is_pointer>(TypeList<int*, bool*, float*, double*>{}));
static_assert(!AllOf<std::is_pointer>(TypeList<int, bool, float, double>{}));
static_assert(AllOf<std::is_integral>(TypeList<int, unsigned, long, long long, short>{}));
static_assert(AnyOf<std::is_pointer>(TypeList<int, bool*, float*, double*>{}));


static_assert (LargestTypeIndex(TypeList<std::string, double, float>{}) == 0);