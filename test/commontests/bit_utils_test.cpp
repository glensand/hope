/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"

#include "components/bit_utils.h"

namespace {
    
    struct st_bit_4 final {
        unsigned _0 : 3, _1 : 9, _2 : 11, _3 : 6;
        float f;
    };

}

TEST(BitFieldTest, Bit4Test)
{
    st_bit_4 st;
    st._0 = ~0;
    st._1 = ~0;
    st._2 = ~0;
    st._3 = ~0;
     
    const auto count_0 = hope::bit_utils::bit_count(st._0);
    ASSERT_TRUE(count_0 == 3);

    const auto count_1 = hope::bit_utils::bit_count(st._1);
    ASSERT_TRUE(count_1 == 9);

    const auto count_2 = hope::bit_utils::bit_count(st._2);
    ASSERT_TRUE(count_2 == 11);

    const auto count_3 = hope::bit_utils::bit_count(st._3);
    ASSERT_TRUE(count_3 == 6);
}