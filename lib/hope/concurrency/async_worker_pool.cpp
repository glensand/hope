#include "async_worker_pool.h"
#include <iostream>

namespace hope::concurrency {

    async_worker_pool::async_worker_pool(std::size_t thread_count)
        : m_thread_count(thread_count)
        , m_pool(thread_count){
        
    }

    void async_worker_pool::run() noexcept {
        for (auto&& thread : m_pool)
            thread.run();
    }

    void async_worker_pool::stop() noexcept {
        for (auto&& thread : m_pool)
            thread.stop();
    }

    void async_worker_pool::shutdown() noexcept {
        for (auto&& thread : m_pool)
            thread.shut_down();
    }

    void async_worker_pool::add_job(Job&& task) noexcept {
        auto thread_index = std::size_t((1 + m_thread_count) * (double)std::rand() / RAND_MAX);
        thread_index = thread_index % m_thread_count;
        m_pool[thread_index].add_job(std::move(task));
    }

    void async_worker_pool::wait() {
        for (auto&& thread : m_pool)
            thread.wait();
    }

}