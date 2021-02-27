/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"
#include "core/static_string.h"

template<std::size_t N, typename T>
struct named_field final {
    hope::static_string<N> name;
    T value;

    named_field(const hope::static_string<N>& _name, const T& _val = T{})
        : name(_name)
        , value(_val) {

    }
};

struct test_query final {
    /*named_field<5, int> protocol_version{ "protocol_version"_ss };*/
};

TEST(StaticStringTest, ConstexprInitialization)
{
 /*   constexpr auto name = "protocol_version"_ss;*/
    test_query query;
}
