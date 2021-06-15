/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"

#include "hope/tuple/flat_tuple.h"
#include "hope/tuple/tuple_from_struct.h"
#include "hope/tuple/tuple_from_struct_unsafe.h"

namespace {

struct pod_imitator {
    constexpr static int DefaultInt{ 11 };
    float val1{ 10.f };
    int val2{ DefaultInt };
};

struct test_struct_3 {
    double _0;
    float _1;
    int _2;
    bool _3;
};

struct pod_imitator_derived final : pod_imitator {
    bool b;
};

struct struct_string final {
    std::string name;
    int index;
};

struct struct_int_vector final {
    std::string name;
    std::vector<int> vec_i;
};

struct struct_string_vector final {
    std::string name;
    std::vector<int> vec_i;
    std::vector<std::string> vec_s;
};

struct struct_virtual_method {
    std::string name1;
    virtual void do_stuff() { std::cout << "struct_virtual_method"; }
};

struct struct_virtual_derived final : struct_virtual_method {
    std::string name2;
    virtual void do_stuff() override { std::cout << "struct_virtual_method"; }
};
}

TEST(TupleTest, ConstexprInitialization)
{
    constexpr auto tuple = hope::make_flat_tuple(1.f, std::string_view("string"), pod_imitator{});

    static_assert(tuple.get<0>() == tuple.get<float>());
    static_assert(tuple.get<1>() == tuple.get<std::string_view>());
    static_assert(tuple.get<2>().val2 == pod_imitator::DefaultInt);
}

TEST(TupleTest, DedactionGuide)
{
    constexpr hope::flat_tuple tuple(1.f, std::string_view("string"), pod_imitator{});

    static_assert(tuple.get<0>() == tuple.get<float>());
    static_assert(tuple.get<1>() == tuple.get<std::string_view>());
    static_assert(tuple.get<2>().val2 == pod_imitator::DefaultInt);
}

TEST(TupleTest, ValueChanging)
{
    auto tuple = hope::make_flat_tuple(pod_imitator{}, pod_imitator{ 0.1f, 1 });
    auto&& secondInt = tuple.get<1>().val2;
    ASSERT_EQ(secondInt, 1);
    secondInt = 12;
    ASSERT_EQ(secondInt, 12);
}

TEST(TupleTest, TupleFromStruct)
{
    constexpr test_struct_3 ts3{ 0.1, 0.1f, 11, true};
    constexpr auto ts3Tuple = hope::tuple_from_struct(ts3);

    static_assert(ts3Tuple.get<2>() == ts3._2);
    static_assert(ts3Tuple.get<3>() == ts3._3);
    ASSERT_TRUE(std::abs(ts3Tuple.get<0>() - ts3._0) < std::numeric_limits<double>::epsilon());
}

TEST(TupleTest, StructDerived)
{
    const pod_imitator_derived d{};

    //auto&& tuple = hope::tuple_from_struct(d);
}

TEST(TupleTest, TupleFromStructUnsafe)
{
    constexpr test_struct_3 ts3{ 0.1, 0.1f, 11, true };
    auto ts3Tuple = hope::tuple_from_struct_unsafe(ts3);

    ASSERT_TRUE(ts3Tuple.get<0>() == ts3._0);
}

TEST(TupleTest, StructStringValue)
{
    struct_string s{ "field1", 11 };
    auto&& tuple = hope::tuple_from_struct(s);

    auto&& first = tuple.get<0>();
    auto&& second = tuple.get<1>();

    ASSERT_TRUE(s.name == first);
    ASSERT_TRUE(s.index == second);
}

TEST(TupleTest, StructVectorIntValue)
{
    const struct_int_vector s{ "field1", {11, 12, 13} };
    auto&& tuple = hope::tuple_from_struct(s);

    auto&& first = tuple.get<0>();
    auto&& second = tuple.get<1>();

    ASSERT_TRUE(s.name == first);

    for (std::size_t i{ 0 }; i < second.size(); ++i)
        ASSERT_TRUE(second[i] == s.vec_i[i]);
}

TEST(TupleTest, StructVectorStringValue)
{
    struct_string_vector s{ "field1", {11, 12, 13}, {"first", "second", "third"} };
    auto&& tuple = hope::tuple_from_struct(s, hope::field_policy::reference{});

    auto&& first = tuple.get<0>();
    auto&& second = tuple.get<1>();
    auto&& third = tuple.get<2>();

    ASSERT_TRUE(s.name == first);

    for (std::size_t i{ 0 }; i < second.size(); ++i)
        ASSERT_TRUE(second[i] == s.vec_i[i]);

    for (std::size_t i{ 0 }; i < third.size(); ++i)
        ASSERT_TRUE(third[i] == s.vec_s[i]);
}

TEST(TupleTest, StructStringReference)
{
    struct_string s{ "field1", 11 };
    auto&& tuple = hope::tuple_from_struct(s, hope::field_policy::reference{});

    auto&& first = tuple.get<0>();
    auto&& second = tuple.get<1>();

    ASSERT_TRUE(s.name == first);
    ASSERT_TRUE(s.index == second);

    first = "field1_modified";
    second = 12;

    ASSERT_TRUE(s.name == "field1_modified");
    ASSERT_TRUE(s.index == 12);
}

TEST(TupleTest, StructVectorIntReference)
{
    struct_int_vector s{ "field1", {11, 12, 13} };
    auto&& tuple = hope::tuple_from_struct(s, hope::field_policy::reference{});

    auto&& first = tuple.get<0>();
    auto&& second = tuple.get<1>();

    ASSERT_TRUE(s.name == first);

    for(std::size_t i{ 0 }; i < second.size(); ++i)
        ASSERT_TRUE(second[i] == s.vec_i[i]);

    first = "field1_modified";
    second.push_back(144);

    ASSERT_TRUE(s.name == "field1_modified");
    ASSERT_TRUE(s.vec_i.back() == 144);
}

TEST(TupleTest, StructVectorStringReference)
{
    struct_string_vector s{ "field1", {11, 12, 13}, {"first", "second", "third"} };
    auto&& tuple = hope::tuple_from_struct(s, hope::field_policy::reference{});

    auto&& first = tuple.get<0>();
    auto&& second = tuple.get<1>();
    auto&& third = tuple.get<2>();

    ASSERT_TRUE(s.name == first);

    for (std::size_t i{ 0 }; i < second.size(); ++i)
        ASSERT_TRUE(second[i] == s.vec_i[i]);

    for (std::size_t i{ 0 }; i < third.size(); ++i)
        ASSERT_TRUE(third[i] == s.vec_s[i]);

    first = "field1_modified";
    second.push_back(144);
    third.emplace_back("last");

    ASSERT_TRUE(s.name == "field1_modified");
    ASSERT_TRUE(s.vec_i.back() == 144);
    ASSERT_TRUE(s.vec_s.back() == "last");
}

TEST(TupleTest, StructVirtualMethodValue)
{
    struct_virtual_method s;
    s.name1 = "name1";
    auto&& tuple = hope::tuple_from_struct(s);
    auto&& first = tuple.get<0>();
    ASSERT_TRUE(s.name1 == first);
}

TEST(TupleTest, StructVirtualMethodDerivedValue)
{
    //struct_virtual_derived s;
    //s.name1 = "name1";
    //s.name2 = "name2";
    //auto&& tuple = hope::tuple_from_struct(s);
    //auto&& first = tuple.get<0>();
    //auto&& second = tuple.get<1>();
    //ASSERT_TRUE(s.name1 == first);
    //ASSERT_TRUE(s.name2 == second);
}

TEST(TupleTest, StructVirtualMethodReference)
{
    //struct_virtual_method s;
    //s.name1 = "name1";
    //auto&& tuple = hope::tuple_from_struct(s, hope::field_policy::reference{});
    //auto&& first = tuple.get<0>();
    //ASSERT_TRUE(s.name1 == first);
    //first = "name1changed";
    //ASSERT_TRUE(s.name1 == "name1changed");
}

TEST(TupleTest, StructVirtualMethodDerivedReference)
{
    //struct_virtual_derived s;
    //s.name1 = "name1";
    //s.name2 = "name2";
    //auto&& tuple = hope::tuple_from_struct(s, hope::field_policy::reference{});
    //auto&& first = tuple.get<0>();
    //auto&& second = tuple.get<1>();
    //ASSERT_TRUE(s.name1 == first);
    //ASSERT_TRUE(s.name2 == second);
    //first = "name1changed";
    //second = "name2changed";
    //ASSERT_TRUE(s.name1 == "name1changed");
    //ASSERT_TRUE(s.name2 == "name2changed");
}