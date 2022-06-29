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

    enum class alloc_policy : uint8_t {
        buffered,
        new_only
    };

    template<typename TItem, alloc_policy policy = alloc_policy::buffered>
    class mpsc_queue final {
    public:
        mpsc_queue(mpsc_queue const&) = delete;
        mpsc_queue& operator = (mpsc_queue const&) = delete;

        mpsc_queue(mpsc_queue&& queue) = default;
        mpsc_queue& operator=(mpsc_queue&& queue) = default;

        explicit mpsc_queue() {
            m_head = m_tail = m_buffer_head = new node();
        }

        ~mpsc_queue() {
            while (m_tail != nullptr) {
                auto* cur_node = m_tail;
                m_tail = m_tail->next;

                delete cur_node;
            }
        }

        template<typename T>
        void enqueue(T&& in_value) {
            node* new_node = nullptr;
            if constexpr (policy == alloc_policy::buffered){
                new_node = alloc_node(std::forward<T>(in_value));
            } else{
                new_node = new node(std::forward<T>(in_value));
            }
            
            auto* old_head = m_head.exchange(new_node);
            old_head->next = new_node;
        }

        bool try_dequeue(TItem& v) {
            auto* popped = m_tail->next;

            if (popped) {
                v = std::move(popped->value);
                auto* old_tail = m_tail;
                m_tail = popped;
                m_tail->value = { };
                if constexpr (policy == alloc_policy::new_only){
                    delete old_tail;
                }
                return true;
            }

            return false;
        }

    private:
        // internal node structure 
        struct node final {
            template<typename T = TItem>
            node(T&& in_value = T{})
                : value(std::forward<T>(in_value)) { }

            node* volatile next = nullptr;
            TItem value;
        };

        template<typename T>
        node* alloc_node(T&& in_value) {
            node* new_node = m_buffer_head.load(std::memory_order_consume);
            while(true){
                if(new_node != m_tail){
                    if(m_buffer_head.compare_exchange_strong(new_node, new_node->next, 
                            std::memory_order_release, std::memory_order_relaxed)) {
                        new_node->value = std::forward<T>(in_value);
                        new_node->next = nullptr;
                        break;
                    }
                } else {
                    new_node = new node(std::forward<T>(in_value));
                    break;
                }
            }

            return new_node;
        }

        // consumer part 
        // accessed mainly by consumer, infrequently be producer 
        node* m_tail = nullptr; // tail of the queue 

        // cache line size on modern x86 processors (in bytes) 
        constexpr static std::size_t CacheLineSize = 64;

        // delimiter between consumer part and producer part
        // is needed to put this parts to the different cache lines
        const uint8_t m_cache_line_pad_[CacheLineSize]{};

        // producer part 
        // accessed only by producer 
        std::atomic<node*> m_buffer_head = nullptr;
        std::atomic<node*> m_head = nullptr; // head of the queue 
    };

}
