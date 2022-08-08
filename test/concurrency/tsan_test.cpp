/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"
#include "hope/concurrency/simple_thread_sanitizer.h"

void test_function() {
    TSAN_SAFE;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void test_function2(int depth) {
    TSAN_SAFE;
    if(depth > 0) {
        test_function2(depth - 1);
    }
}

TEST(TsanTest, Contenuation)
{
    std::thread t1([] {test_function(); test_function(); });
    //std::thread t2([] {test_function(); });
    t1.join();
    //t2.join();
}

TEST(TsanTest, Recursion)
{
    std::thread t1([] {test_function2(10); });
    t1.join();
    test_function2(1);
}