/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <atomic>

namespace hope::concurrency{

    class spin_lock {

    public:
        spin_lock() = default;
        ~spin_lock() = default;

        /**
         * \brief Tries to acquire atomic flag in cycle; Checks it on every iteration;
         */
        void lock() {
            while (m_lockFlag.test_and_set(std::memory_order_acquire)) { }
        }

        /**
         * \brief Tries to acquire atomic flag at once, return true in succeed 
         * \return 
         */
        bool try_lock() {
            return !m_lockFlag.test_and_set(std::memory_order_acquire);
        }

        /**
         * \brief Releases atomic flag.
         */
        void unlock() {
            m_lockFlag.clear();
        }

    private:
        std::atomic_flag m_lockFlag;
    };
    
}
