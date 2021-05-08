/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"

#include "hope/tuple/flat_tuple.h"
#include "hope/tuple/tuple_from_struct.h"

namespace detail {
    struct test_struct final {
        int _0;
        float _1;
        double _2;
    };
}

TEST(StructToRefTuple, AllCvModifiedTypesAreEqual)
{
    constexpr detail::test_struct instance{ 1, 1.1f, 1.3 };
    constexpr auto ref = hope::ref_tuple_from_struct(instance);
    static_assert(ref.get<0>() == ref.get<int>());
    static_assert(ref.get<0>() == ref.get<int&>());
    static_assert(ref.get<0>() == ref.get<const int&>());
    static_assert(ref.get<0>() == ref.get<const int>());

    static_assert(ref.get<1>() == ref.get<float>());
    static_assert(ref.get<1>() == ref.get<float&>());
    static_assert(ref.get<1>() == ref.get<const float&>());
    static_assert(ref.get<1>() == ref.get<const float>());
}

TEST(StructToRefTuple, StructureCanBeModifiedViaTuple)
{
    detail::test_struct instance{ 1, 1.1f, 1.3 };
    auto&& ref = hope::ref_tuple_from_struct(instance);
    ref.get<0>() = 42;
    ref.get<1>() = 42.f;

    ASSERT_TRUE(42 == instance._0 == ref.get<0>() == ref.get<int>());
    ASSERT_TRUE(42.f == instance._1 == ref.get<1>() == ref.get<float>());

}
