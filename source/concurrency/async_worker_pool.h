/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "concurrency/async_worker.h"

namespace hope::concurrency {

    template<std::size_t ThreadCount>
    class async_worker_pool final {
        using WorkersPool = std::array<async_worker, ThreadCount>;
        using Job = std::function<void()>;
    public:
        async_worker_pool() noexcept = default;

        void run() noexcept;

        void stop() noexcept;

        void shutdown() noexcept;

        void add_job(Job&& task) noexcept;

    private:
        WorkersPool m_pool;
    };

    template <std::size_t ThreadCount>
    void async_worker_pool<ThreadCount>::run() noexcept {
        for (auto&& thread : m_pool)
            thread.run();
    }

    template <std::size_t ThreadCount>
    void async_worker_pool<ThreadCount>::stop() noexcept {
        for (auto&& thread : m_pool)
            thread.stop();
    }

    template <std::size_t ThreadCount>
    void async_worker_pool<ThreadCount>::shutdown() noexcept {
        for (auto&& thread : m_pool)
            thread.shutdown();
    }

    template <std::size_t ThreadCount>
    void async_worker_pool<ThreadCount>::add_job(Job&& task) noexcept {
        std::size_t thread_index = std::rand() / ThreadCount;
        m_pool[thread_index].add_job(std::move(task));
    }
}
