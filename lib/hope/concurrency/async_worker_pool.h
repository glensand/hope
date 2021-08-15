/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/concurrency/async_worker.h"
#include <vector>

namespace hope::concurrency {

    class async_worker_pool final {
        using WorkersPool = std::vector<async_worker>;
    public:

        enum class job_add_policy : std::size_t {
            sequential = 0,
            random,
            count,
        };

        using job_t = async_worker::job_t;

        explicit async_worker_pool(std::size_t thread_count = std::thread::hardware_concurrency() + 1);

        /**
         * \brief Runs all the recently created threads
         */
        void run() noexcept;

        /**
         * \brief Stops all the controlled threads, completes jobs 
         */
        void stop() noexcept;

        /**
         * \brief Stops all the controlled threads, when this method will be called
         * all jobs will bve discarded
         */
        void shutdown() noexcept;

        /**
         * \brief Adds given job to the random controlled thread
         * \param task Job to be added
         * \param add_policy
         */
        void add_job(job_t&& task, job_add_policy add_policy = job_add_policy::random) noexcept;

        /**
         * \brief Synchronous operation, wait while all recently added jobs will be completed.
         * This method is not intended to be called from workers
         */
        void wait();
    private:
        WorkersPool m_pool;

        std::size_t m_last_thread_index = 0;
        const std::size_t m_thread_count;
    };

}
