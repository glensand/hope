/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <atomic>

#undef mm_pause
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#   include <immintrin.h>
#   define mm_pause _mm_pause()
#else
#   define mm_pause
#endif

namespace hope::concurrency{

    class spin_lock {

    public:
        spin_lock() = default;
        ~spin_lock() = default;

        void lock() noexcept {
            for (;;) {
                // Optimistically assume the lock is free on the first try
                if (!m_lock.exchange(true, std::memory_order_acquire)) {
                    return;
                }
                // Wait for lock to be released without generating cache misses
                while (m_lock.load(std::memory_order_relaxed)) {
                    // Issue X86 PAUSE or ARM YIELD instruction to reduce contention between
                    // hyper-threads
                    mm_pause;
                }
            }
        }

        bool try_lock() noexcept {
            // First do a relaxed load to check if lock is free in order to prevent
            // unnecessary cache misses if someone does while(!try_lock())
            return !m_lock.load(std::memory_order_relaxed) &&
                !m_lock.exchange(true, std::memory_order_acquire);
        }

        void unlock() noexcept {
            m_lock.store(false, std::memory_order_release);
        }

    private:
        std::atomic_bool m_lock = { false };
    };
    
}
