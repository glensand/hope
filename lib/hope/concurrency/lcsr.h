/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <atomic>

namespace hope::concurrency {

    // load with 'consume' (data-dependent) memory ordering 
    template<typename T>
    T load_consume(const T * addr) {
        // hardware fence is implicit on x86 
        T v = *const_cast<T const volatile*>(addr);
        std::atomic_signal_fence(std::memory_order_seq_cst);
        return v;
    }

    // store with 'release' memory ordering 
    template<typename T>
    void store_release(T* addr, T v) {
        // hardware fence is implicit on x86 
        std::atomic_signal_fence(std::memory_order_seq_cst);
        *const_cast<T volatile*>(addr) = v;
    }

}
