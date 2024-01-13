/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"

#include "hope/components/any_convertible.h"

struct struct_a_t : hope::any_convertible<struct_a_t>{
    int index;
};

struct struct_b_t {
    struct_b_t& operator=(const struct_a_t& s) {
        index = s.index;
        return *this;
    }
    int index;
};

const struct_b_t& just_take(const struct_b_t& b) {
    return b;
}

TEST(AnyConertible, BaseTest1)
{
    struct_a_t a{ };
    a.index = 11;
    const auto b = just_take(a);
    ASSERT_TRUE(b.index == 11);
}

TEST(AnyConertible, BaseTest2)
{
    struct struct_b_inner_t {
        static struct_b_inner_t from(const struct_a_t& s) {
            return { s.index };
        }

        int index;
    };     

    struct_a_t a{ };
    a.index = 11;
    struct_b_inner_t b = a;
    ASSERT_TRUE(b.index == 11);
}
