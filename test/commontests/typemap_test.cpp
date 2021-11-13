/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "hope/components/typemap.h"
#include "hope/typelist/type_value_map.h"
#include "gtest/gtest.h"
#include <string_view>

namespace {

    struct int_value final { int value; };
    struct float_value final { float value; };
    struct string_value final { std::string_view value; };
    struct bool_value final { bool value; };

    using map_t = hope::type_map<
        hope::type_pair<int, int_value>,
        hope::type_pair<float, float_value>,
        hope::type_pair<std::string_view, string_value>,
        hope::type_pair<bool, bool_value>
    >;

    template <typename T1, typename T2>
    constexpr bool is_same(hope::type_holder<T1>, T2) {
        return std::is_same_v<T1, T2>;
    }

    static_assert(is_same(map_t::get<int>(), int_value{ }));
    static_assert(is_same(map_t::get<float>(), float_value{ }));
    static_assert(is_same(map_t::get<std::string_view>(), string_value{ }));
    static_assert(is_same(map_t::get<bool>(), bool_value{ }));

    struct simple_struct final{
        const std::string v1{"TestString"};
    };

    hope::type_value_map map(
        hope::tv<int>(1),
        hope::tv<float>("float"), 
        hope::tv<std::string>(1),
        hope::tv<double>(simple_struct{})
    );
}

TEST(TypeMapTest, GetTest){
    ASSERT_TRUE(map.get<int>() == 1);
    ASSERT_TRUE(map.get<float>() == std::string("float"));
    ASSERT_TRUE(map.get<std::string>() == 1);
    ASSERT_TRUE(map.get<double>().v1 == simple_struct{}.v1);
}

TEST(TypeMapTest, SetTest){
    map.set<int>(11);
    ASSERT_TRUE(map.get<int>() == 11);
}