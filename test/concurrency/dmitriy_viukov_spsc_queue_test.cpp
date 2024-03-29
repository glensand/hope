/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"
#include "hope/concurrency/spsc_queue.h"
#include "hope/concurrency/mpsc_queue.h"
#include "hope/concurrency/mpmc_bounded_queue.h"
#include "hope/concurrency/compound_queue.h"

#include <array>

TEST(ViukovSpsc, Initi)
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

TEST(Mpsc, Compilation)
{
    hope::concurrency::mpsc_queue<int> queue;
    queue.enqueue(4);
    queue.enqueue(1);
    int val;
    queue.try_dequeue(val);
    ASSERT_TRUE(val == 4);
    queue.try_dequeue(val);
    ASSERT_TRUE(val == 1);
}

TEST(CompoundQueue, Compilation)
{
    hope::concurrency::compound_queue<int> queue;
    queue.register_thread(std::this_thread::get_id());
    queue.enqueue(4);
    queue.enqueue(1);
    int val;
    queue.try_dequeue(val);
    ASSERT_TRUE(val == 4);
    queue.try_dequeue(val);
    ASSERT_TRUE(val == 1);
}

TEST(MpSc, Threading)
{
    constexpr static auto threads_count = 5;
    std::array<std::vector<int>, threads_count> buffer;
    for(unsigned i = 0; i < threads_count; ++i) {
        for(unsigned j = 0; j < 100; ++j){
            buffer[i].push_back(std::rand());
        }
    }

    hope::concurrency::mpsc_queue<int> queue;
    std::vector<std::thread> threads;
    for(unsigned i = 0; i < threads_count; ++i){
        threads.emplace_back([i, &buffer, &queue]{
            for(auto&& item : buffer[i]){
                queue.enqueue(item);
            }
        });
    }

    for(unsigned i = 0; i < 100 * threads_count;){
        int stb = 0;
        if(queue.try_dequeue(stb)){
            ++i;
        }
    }

    for(auto&& t : threads)
        t.join();
}

TEST(MpMc, Threading)
{
    constexpr static auto generated_objects = 100;

    constexpr static auto threads_count = 5;
    std::vector<int> buffer;
    for (unsigned j = 0; j < generated_objects; ++j) {
        buffer.push_back(std::rand());
    }

    hope::concurrency::mpmc_bounded_queue<int> queue(1024);
    std::vector<std::thread> producers;
    for (unsigned i = 0; i < threads_count; ++i) {
        producers.emplace_back([&buffer, &queue] {
            for (auto&& item : buffer) {
                queue.try_enqueue(item);
            }
        });
    }

    std::vector<std::thread> consumers;
    for(unsigned i = 0; i < threads_count; ++i) {
        consumers.emplace_back([&queue] {
            for (unsigned i = 0; i < generated_objects;) {
                int stb = 0;
                if (queue.try_dequeue(stb)) {
                    ++i;
                }
            }
        });
    }

    for (auto&& t : producers)
        t.join();

    for (auto&& t : consumers)
        t.join();
}