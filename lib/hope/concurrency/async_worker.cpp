#include "hope/concurrency/async_worker.h"

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

    async_worker::async_worker() noexcept {
        m_wait.store(false, std::memory_order_release);
    }

    void async_worker::run() noexcept {
        m_launched.store(true, std::memory_order_release);
        m_shut_down.test_and_set(std::memory_order_release);
        m_thread_impl = std::thread([this] {
            run_impl();
        });
    }

    void async_worker::run_impl () noexcept {
        for(; m_launched.load(std::memory_order_acquire);) {
            m_wait.store(false, std::memory_order_release);
            job_t job;
            while(m_job_queue.try_dequeue(job)) {
                if(job)
                    job();
            }

            if (!m_shut_down.test_and_set(std::memory_order_acquire))
                return;

            m_wait.store(true, std::memory_order_release);
            m_jobs_complete.set();
            (void)m_job_added.wait();
        }
    }

    void async_worker::add_job(job_t&& task) noexcept {
        m_job_queue.enqueue(std::move(task));
        m_job_added.set();
    }

    void async_worker::wait() const noexcept {
        if(!m_wait.load(std::memory_order_acquire)) {
            (void)m_jobs_complete.wait();
        }
    }
}
