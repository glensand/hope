/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"

#include "hope/components/utility.h"
#include "hope/components/user_defined_types.h"

using namespace hope;

enum test_enum_1 : unsigned {
    val_0 = 0,
    val_1,
    val_2,
    val_3,
};

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)

static_assert(any(val_0, val_1, val_2) == val_1);
static_assert(!(any(val_0, val_1, val_2) == val_3));

static_assert(val_1 == any(val_0, val_1, val_2));
static_assert(!(val_3 == any(val_0, val_1, val_2)));

static_assert(!(all(val_0, val_1, val_2) == val_3));
static_assert(all(val_3, val_3, val_3) == val_3);
static_assert(all(val_0, val_1, val_2) != val_3);

static_assert(!(val_3 == all(val_0, val_1, val_2)));
static_assert(val_3 == all(val_3, val_3, val_3));
static_assert(val_3 != all(val_0, val_1, val_2));

static_assert(hope::is_string_v<std::string>);
static_assert(hope::is_string_v<std::wstring>);
static_assert(hope::is_vector_v<std::vector<bool>>);
static_assert(hope::is_vector_v<std::vector<int>>);
static_assert(hope::is_array_v<std::array<int, 1>>);
static_assert(hope::is_array_v<std::array<int, 0>>);

struct user_type final {
    std::string string;
};

static_assert(hope::is_user_defined_type_v<user_type>);

TEST(AnyTest, String)
{
    // TODO:: make code compilable on gcc
    const std::string value{ "Variant" };
    const std::string value2{ "Va riant" };
    ASSERT_TRUE(hope::any("Variant", "Variant2", "Variant3") == value);
    ASSERT_TRUE(hope::all("Variant", "Variant2", "Variant3") != value2);
    ASSERT_TRUE(!(hope::all("Variant", "Variant2", "Variant3") == value2));
}

#endif