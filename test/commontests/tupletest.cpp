/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"

#include "tuple/flat_tuple.h"
#include "tuple/tuple_from_struct.h"
#include "tuple/tuple_from_struct_safe.h"

struct pod_imitator {
    constexpr static int DefaultInt{ 11 };
    float val1{ 10.f };
    int val2{ DefaultInt };
};

struct complicated_struct {
    std::string_view name;
    int index;
};

TEST(TupleTest, ConstexprInitialization)
{
    constexpr auto tuple = hope::make_flat_tuple(1.f, std::string_view("string"), pod_imitator{});

    static_assert(tuple.get<0>() == tuple.get<float>());
    static_assert(tuple.get<1>() == tuple.get<std::string_view>());
    static_assert(tuple.get<2>().val2 == pod_imitator::DefaultInt);
}

TEST(TupleTest, ValueChanging)
{
    auto tuple = hope::make_flat_tuple(pod_imitator{}, pod_imitator{ 0.1, 1 });
    auto&& secondInt = tuple.get<1>().val2;
    ASSERT_EQ(secondInt, 1);
    secondInt = 12;
    ASSERT_EQ(secondInt, 12);
}

struct test_struct_3 {
    double _0;
    float _1;
    int _2;
    bool _3;
};

TEST(TupleTest, TupleFromStruct)
{
    constexpr test_struct_3 ts3{ 0.1, 0.1f, 11, true};
    constexpr auto ts3Tuple = hope::tuple_from_struct(ts3);

    static_assert(ts3Tuple.get<2>() == ts3._2);
    static_assert(ts3Tuple.get<3>() == ts3._3);
    ASSERT_TRUE(std::abs(ts3Tuple.get<0>() - ts3._0) < std::numeric_limits<double>::epsilon());
}

TEST(TupleTest, TupleFromStructUnsafe)
{
    constexpr test_struct_3 ts3{ 0.1, 0.1f, 11, true };
    auto ts3Tuple = hope::tuple_from_struct_unsafe(ts3);

    ASSERT_TRUE(ts3Tuple.get<0>() == ts3._0);
}
