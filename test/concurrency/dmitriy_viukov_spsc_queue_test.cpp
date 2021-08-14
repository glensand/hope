/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"
#include "hope/concurrency/spsc_queue.h"

TEST(ViukovSpscTest, Initi)
{
    hope::concurrency::spsc_queue<int> queue;
    queue.enqueue(4);
    queue.enqueue(1);
    int val;
    queue.try_dequeue(val);
    ASSERT_TRUE(val == 4);
    queue.try_dequeue(val);
    ASSERT_TRUE(val == 1);
}
