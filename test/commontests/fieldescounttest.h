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