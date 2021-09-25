/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"
#include "hope/concurrency/async_worker.h"
#include "hope/concurrency/async_worker_pool.h"

namespace
{

    struct changed final {
        std::atomic_bool test;
    };

}

TEST(WorkerTest, OneJob)
{
    return;
    changed test_s;
    test_s.test = false;
    hope::concurrency::async_worker worker;
    worker.run();
    worker.add_job([&] {
        test_s.test.store(true, std::memory_order_release);
    });
    worker.wait();
    worker.stop();
    ASSERT_TRUE(test_s.test.load());
}

TEST(WorkerTest, Wait)
{
    return;
    std::vector<int> ints;
    for (auto i = 0; i < 100; ++i)
        ints.push_back(i);

    hope::concurrency::async_worker worker;
    worker.run();
    for (auto i = 0; i < 3; ++i)
        worker.add_job([&] {
            int sum = 0;
            for (auto i : ints)
                sum += i;
    });
    worker.wait();
    worker.stop();
}
