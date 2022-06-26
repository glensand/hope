/* Copyright (C) 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

// The implementation is based on:
// https://www.1024cores.net/home/lock-free-algorithms/queues/non-intrusive-mpsc-node-based-queue

#pragma once

#include <atomic>

namespace hope::concurrency {

    template<typename T>
    class mpsc_queue final {
    public:

        mpsc_queue(mpsc_queue const&) = delete;
        mpsc_queue& operator = (mpsc_queue const&) = delete;

        mpsc_queue(mpsc_queue&& queue) = default;
        mpsc_queue& operator=(mpsc_queue&& queue) = default;

        explicit mpsc_queue() {
            m_head = m_tail = new node();
        }

        ~mpsc_queue() {
            while (m_tail != nullptr) {
                auto* cur_node = m_tail;
                m_tail = m_tail->next;

                delete cur_node;
            }
        }

        template<typename... Args>
        void enqueue(Args&&... v) {
            auto* n = new node(std::forward<Args>(v)...);
            auto* old_head = m_head.exchange(n);
            old_head->next = n;
        }

        bool try_dequeue(T& v) {
            auto* popped = m_tail->next;

            if (popped) {
                v = std::move(popped->value);
                auto* old_tail = m_tail;
                m_tail = popped;
                m_tail->value = { };
                delete old_tail;
                return true;
            }

            return false;
        }

    private:
        // internal node structure 
        struct node final {
            template<typename... Args>
            node(Args&&... args)
                : value(std::forward<Args>(args)...) { }

            node* volatile next = nullptr;
            T value;
        };

        // consumer part 
        // accessed mainly by consumer, infrequently be producer 
        node* m_tail = nullptr; // tail of the queue 

        // cache line size on modern x86 processors (in bytes) 
        constexpr static std::size_t  CacheLineSize = 64;

        // delimiter between consumer part and producer part
        // is needed to put this parts to the different cache lines
        const uint8_t m_cache_line_pad_[CacheLineSize]{};

        // producer part 
        // accessed only by producer 
        std::atomic<node*> m_head = nullptr; // head of the queue 
    };

}
