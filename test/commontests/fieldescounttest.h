#pragma once

#include "tuple/DetectFieldsCount.h"

struct ZeroFields
{
    
};

static_assert(DetectFieldsCount(ZeroFields{}) == 0);

struct OneField
{
    int x;
};

static_assert(DetectFieldsCount(OneField{}) == 1);

struct ThirdFields
{
    int a, b, c;
};

static_assert(DetectFieldsCount(ThirdFields{}) == 3);