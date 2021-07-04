/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <deque>
#include "hope/concurrency/spin_lock.h"
#include <mutex>

namespace hope::concurrency {

    template <typename T>
    class queue {
    public:
        queue() = default;
        queue(queue&&) = default;
        queue& operator=(queue&&) = default;
        queue(const queue&) = default;
        queue& operator=(const queue&) = default;
        ~queue() = default;

        template <typename... Args>
        void push(Args&&... args) {
            const std::lock_guard lock(m_lock);
            m_queue_impl.emplace_back(std::forward<Args...>(args...));
        }

        T pop() {
            const std::lock_guard lock(m_lock);
            auto top = m_queue_impl.front();
            m_queue_impl.pop_front();
            return top;
        }

        bool empty() const noexcept {
            const std::lock_guard lock(m_lock);
            return m_queue_impl.empty();
        }

    private:
        mutable spin_lock m_lock;
        std::deque<T> m_queue_impl;
    };

}
