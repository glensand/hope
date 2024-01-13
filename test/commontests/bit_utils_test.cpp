/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"

#include "hope/components/bit_utils.h"

namespace {
    
    struct st_bit_4 final {
        unsigned _0 : 3, _1 : 9, _2 : 11, _3 : 6;
        float f;
    };

}

TEST(BitFieldTest, Bit4Test)
{
    st_bit_4 st{};
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

namespace
{

void equal_test(unsigned count) {
    hope::bit_utils::bit_mask mask(count);

    for (unsigned i = 0; i < count; ++i)
        mask.set(i, true);

    for (unsigned i = 0; i < count; ++i)
        ASSERT_TRUE(mask.get(i));
}

void zero_test(unsigned count) {
    hope::bit_utils::bit_mask mask(count);

    for (unsigned i = 0; i < count; ++i)
        ASSERT_TRUE(!mask.get(i));
}

void nth_bit_test(unsigned i) {
    hope::bit_utils::bit_mask mask(i + 1);
    mask.set(i, true);
    ASSERT_TRUE(mask.get(i));
}

void nth_bit_test2(unsigned i) {
    hope::bit_utils::bit_mask mask(111);
    mask.set(i, true);
    ASSERT_TRUE(mask.get(i));
}

}

TEST(BitMaskTest, EqualMaskTest)
{
    for (unsigned i = 0; i < 100; ++i)
        equal_test(i);
}

TEST(BitMaskTest, ZeroMaskTest)
{
    for (unsigned i = 0; i < 100; ++i)
        zero_test(i);
}

TEST(BitMaskTest, NthBitTest)
{
    for (unsigned i = 0; i < 100; ++i)
        nth_bit_test(i);
}

TEST(BitMaskTest, NthBitTest2)
{
    for (unsigned i = 0; i < 100; ++i)
        nth_bit_test2(i);
}

TEST(BitMaskTest, RandomTest)
{
    hope::bit_utils::bit_mask mask(100);
    auto&& pseudo_random = { 1, 13, 6, 23, 9, 66, 53 };
    for (auto r : pseudo_random)
        mask.set(r, true);

    for (auto r : pseudo_random)
        ASSERT_TRUE(mask.get(r));
}

TEST(BitMaskTest, AverageTest)
{
    hope::bit_utils::bit_mask mask(6);
    mask.set(0, true);
    mask.set(4, true);
    ASSERT_TRUE(mask.get(0) && mask.get(4));
    ASSERT_TRUE(!mask.get(1) && !mask.get(2) && !mask.get(3) && !mask.get(5));
}
