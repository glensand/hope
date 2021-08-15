/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/concurrency/spsc_queue.h"
#include "hope/concurrency/event.h"
#include <functional>
#include <thread>

namespace hope::concurrency {

    class async_worker final
    {
    public:
        using job_t = std::function<void()>;
        async_worker() noexcept;

        /**
         * \brief Starts the asynchronous worker, after calling this method you can add new job
         * via add_job
         */
        void run() noexcept;

        /**
         * \brief Synchronously wait while all recently added jobs will be completed, and after it shut down
         * inner thread
         */
        void stop() noexcept;

        /**
         * \brief Unlike method "Stop" this method does not wait any operation, and stops worker's activity
         * as soon as possible, all the remaining jobs will be discarded
         */
        void shut_down() noexcept;

        /**
         * \brief Adds new job to the operation queue, the job may be an empty function, in this case it will be
         * just ignored..
         * \param task the job to be added
         */
        void add_job(job_t&& task) noexcept;

        /**
         * \brief Waits while all the recently added jobs will be completed
         */
        void wait() const noexcept;
    private:
        void run_impl() noexcept;

        std::atomic_flag m_shut_down;
        std::atomic_bool m_wait;
        std::atomic_bool m_launched;
        spsc_queue<job_t> m_job_queue;
        std::thread m_thread_impl;

        auto_reset_event m_job_added;
        auto_reset_event m_jobs_complete;
    };

}
