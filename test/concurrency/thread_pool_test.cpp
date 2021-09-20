/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"
#include "hope/concurrency/async_worker_pool.h"

struct change final {
    std::atomic_bool changed;
    change(bool val) {
        changed = val;
    }

    change(change&& rhs) noexcept {
        changed = rhs.changed.load();
    }
};

TEST(WorkerPoolTest, ChangeTest)
{
    constexpr unsigned TestCount{ 1000 };
    std::vector<change> test_v;
    for(unsigned i = 0; i < TestCount; ++i) {
        test_v.emplace_back(false);
    }

    hope::concurrency::async_worker_pool pool;
    pool.run();
    for(auto&& c : test_v) {
        pool.add_job([&] { c.changed = true; });
    }

    pool.wait();
    for (auto&& c : test_v)
        ASSERT_TRUE(c.changed == true);
    pool.stop();
}

TEST(WorkerPoolTest, Stress)
{
	return;
    std::vector<int> ints;
    for (auto i = 0; i < 1000; ++i)
        ints.push_back(i);

    hope::concurrency::async_worker_pool pool;
    pool.run();
    for (auto i = 0; i < 300; ++i)
        pool.add_job([&] {
        int sum = 0;
        for (auto i : ints)
            sum += i;
    });
    pool.wait();
    pool.stop();
}