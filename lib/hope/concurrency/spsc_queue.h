/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

// The implementation is based on:
// http://www.1024cores.net/home/lock-free-algorithms/queues/unbounded-spsc-queue
// http://software.intel.com/en-us/articles/single-producer-single-consumer-queue)

#pragma once

#include "hope/concurrency/lcsr.h"

namespace hope::concurrency {

    template<typename T>
    class spsc_queue final {
    public:

        spsc_queue(spsc_queue const&) = delete;
        spsc_queue& operator = (spsc_queue const&) = delete;

        spsc_queue(spsc_queue&& queue) = default;
        spsc_queue& operator=(spsc_queue&& queue) = default;

        explicit spsc_queue(std::size_t pre_alloc = 0) {
            node* n = new node;
            m_tail = m_head = m_first = m_m_tail_copy = n;

            // TODO:: rework
            T dummy;
            for (std::size_t i = 0; i != pre_alloc; ++i)
                enqueue(dummy);

            for (std::size_t i = 0; i != pre_alloc; ++i)
                try_dequeue(dummy);
        }

        ~spsc_queue() {
            for(auto* cur_node = m_first; cur_node != nullptr;) {
                auto* next = cur_node->next;
                delete cur_node;
                cur_node = next;
            }
        }

        template<typename... Args>
        void enqueue(Args&&... v)
        {
            auto* n = alloc_node(std::forward<Args>(v)...);
            n->next = nullptr;
            store_release(&m_head->next, n);
            m_head = n;
        }

        bool try_dequeue(T& v) {
            if (load_consume(&m_tail->next))
            {
                v = m_tail->next->value;
                store_release(&m_tail, m_tail->next);
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

            node* next = nullptr;
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
        node* m_head = nullptr; // head of the queue 
        node* m_first = nullptr; // last unused node (tail of node cache) 
        node* m_m_tail_copy = nullptr; // helper (points somewhere between m_first and m_tail) 

        template<typename... Args>
        node* create_from_internal(Args&&... args) {
            node* n = m_first;
            m_first = m_first->next;
            n->value = T(std::forward<Args>(args)...);
            return n;
        }

        template<typename... Args>
        node* alloc_node(Args&&... args) {
            // first tries to allocate node from internal node cache, 
            // if attempt fails, allocates node via ::operator new() 

            if (m_first != m_m_tail_copy)
                return create_from_internal(std::forward<Args>(args)...);

            m_m_tail_copy = load_consume(&m_tail);

            if (m_first != m_m_tail_copy)
                return create_from_internal(std::forward<Args>(args)...);

            return new node(std::forward<Args>(args)...);
        }
    };

}
