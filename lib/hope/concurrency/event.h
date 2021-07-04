/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <limits>

#if !defined(_WIN32) && !defined(_WIN64)
#include <condition_variable>
#include <mutex>
#endif

namespace hope::concurrency {

    class synchronization_event {
    protected:
        enum class policy : unsigned {
            Auto,
            Manual,
        };

        explicit synchronization_event(policy policy) noexcept;
    public:

        ~synchronization_event() noexcept;

        void set() const noexcept;

        void reset() const noexcept;

        bool wait(long waiting_time = std::numeric_limits<long>::max()) const noexcept;

        synchronization_event(synchronization_event&&) = delete;
        synchronization_event(const synchronization_event&) = delete;
        synchronization_event& operator=(synchronization_event&&) = delete;
        synchronization_event& operator=(const synchronization_event&) = delete;

    private:
#if defined(_WIN32) || defined(_WIN64)
        using handle = void*;
        handle m_event;
#else
        mutable std::condition_variable m_cv;
        mutable std::mutex m_mutex;
#endif
    };

    class auto_reset_event final : public synchronization_event {
    public:
        explicit auto_reset_event();

        ~auto_reset_event();

        auto_reset_event(auto_reset_event&&) = delete;
        auto_reset_event(const auto_reset_event&) = delete;
        auto_reset_event& operator=(auto_reset_event&&) = delete;
        auto_reset_event& operator=(const auto_reset_event&) = delete;
    };

    class manual_reset_event final : public synchronization_event {
    public:
        explicit manual_reset_event();

        ~manual_reset_event();

        manual_reset_event(manual_reset_event&&) = delete;
        manual_reset_event(const manual_reset_event&) = delete;
        manual_reset_event& operator=(manual_reset_event&&) = delete;
        manual_reset_event& operator=(const manual_reset_event&) = delete;
    };
}
