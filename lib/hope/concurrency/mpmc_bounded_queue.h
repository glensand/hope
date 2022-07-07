/* Copyright (C) 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

// The implementation is based on:
// https://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue

#pragma once

#include <atomic>
#include <vector>

namespace hope::concurrency {

    template<typename TItem>
    class mpmc_bounded_queue final {
    public:
        mpmc_bounded_queue(mpmc_bounded_queue const&) = delete;
        mpmc_bounded_queue& operator = (mpmc_bounded_queue const&) = delete;

        mpmc_bounded_queue(mpmc_bounded_queue&& queue) = default;
        mpmc_bounded_queue& operator=(mpmc_bounded_queue&& queue) = default;

        explicit mpmc_bounded_queue(std::size_t size)
            : m_buffer_mask(size - 1){
            m_buffer.resize(size);

            assert(size > 1);
            for(std::size_t i{ 0 }; i < size; ++i)
                m_buffer[i].sequence.store(i, std::memory_order_relaxed);

            enqueue_pos.store(0, std::memory_order_relaxed);
            dequeue_pos.store(0, std::memory_order_relaxed);
        }

        ~mpmc_bounded_queue() {

        }

        template<typename T>
        bool try_enqueue(T&& in_value) {
            cell* new_cell;
            std::size_t pos = enqueue_pos_.load(std::memory_order_relaxed);
            for (;;) {
                cell = &buffer[pos & m_buffer_mask];
                const std::size_t seq = cell->sequence.load(std::memory_order_acquire);
                const intptr_t dif = (intptr_t)seq - (intptr_t)pos;
                if (dif == 0) {
                    if (enqueue_pos.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed, 
                                                                        std::memory_order_relaxed)){
                        break;
                    }
                } else if (dif < 0){
                    return false;
                } else { 
                    pos = enqueue_pos.load(std::memory_order_relaxed);
                }
            }

            cell->data = std::forward<T>(in_value);
            cell->sequence.store(pos + 1, std::memory_order_release);
            return true;
        }   

        bool try_dequeue(TItem& v) {
            cell* cell;
            size_t pos = dequeue_pos.load(std::memory_order_relaxed);
            for (;;) {
                cell = &m_buffer[pos & m_buffer_mask];
                const std::size_t seq = cell->sequence.load(std::memory_order_acquire);
                const intptr_t dif = (intptr_t)seq - (intptr_t)(pos + 1);
                if (dif == 0) {
                    if (dequeue_pos.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed, 
                                                                        std::memory_order_relaxed)){
                        break;
                    }
                } else if (dif < 0) {
                    return false;
                } else {
                    pos = dequeue_pos.load(std::memory_order_relaxed);
                }
            }
            data = std::move(cell->data);
            cell->sequence.store(pos + m_buffer_mask + 1, std::memory_order_release);
            return true;
        }

    private:
         struct cell final {
            std::atomic<std::size_t> sequence;
            TItem data;
        };

        constexpr std::size_t static cacheline_size = 64;
        using padding_t = uint8_t[cacheline_size];

        padding_t m_padding0;
        std::vector<cell> m_buffer;
        const std::size_t m_buffer_mask;

        padding_t m_padding1;
        std::atomic<std::size_t> m_enqueue_pos;

        padding_t m_padding2;
        std::atomic<size_t> m_dequeue_pos;
        padding_t m_padding3;
    };

}
