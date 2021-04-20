/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <mutex>
#include "hope/concurrency/spin_lock.h"

namespace hope {

    namespace detail {
        template <typename SingletonImpl, typename Mutex>
        struct lock final {
            
            explicit lock() noexcept {
                m_lock.lock();
            }

            ~lock() noexcept {
                m_lock.unlock();
            }

        private:
            static Mutex m_lock;
        };
    }

    template <typename SingletonImpl>
    struct single_threaded final {
        using VolatileType = SingletonImpl;
        using Lock = int;
    };

    template <typename SingletonImpl>
    struct multi_threaded_spin_lock final {
        using VolatileType = volatile SingletonImpl;
        using Lock = detail::lock<SingletonImpl, concurrency::spin_lock>;
    };

    template <typename SingletonImpl>
    struct multi_threaded_mutex final {
        using VolatileType = volatile SingletonImpl;
        using Lock = detail::lock<SingletonImpl, std::mutex>;
    };

}
