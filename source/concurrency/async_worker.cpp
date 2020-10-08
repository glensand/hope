#include "concurrency/async_worker.h"

namespace hope::concurrency {

    void async_worker::stop() noexcept {
        m_launched.store(false, std::memory_order_release);
        m_job_added.set();
        m_thread_impl.join();
    }

    void async_worker::shut_down() noexcept {
        m_launched.store(false, std::memory_order_release);
        m_shut_down.clear(std::memory_order_release);
        stop();
    }

    void async_worker::run() noexcept {
        m_launched.store(true, std::memory_order_release);
        m_shut_down.test_and_set(std::memory_order_release);
        m_thread_impl = std::thread([=] {
            run_impl();
            });
    }

    void async_worker::run_impl () noexcept {
        for(; m_launched.load(std::memory_order_acquire);) {
            while(!m_job_queue.empty()) {
                auto&& job = m_job_queue.pop();
                job();
            }

            if (!m_shut_down.test_and_set(std::memory_order_acquire))
                return;

            (void)m_job_added.wait();
        }
    }

    void async_worker::add_job(job&& task) noexcept {
        m_job_queue.emplace(std::move(task));
        m_job_added.set();
    }
}
