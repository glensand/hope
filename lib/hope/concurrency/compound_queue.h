/* Copyright (C) 2021 - 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/concurrency/spsc_queue.h"
#include <unordered_map>
#include <thread>
#include <cassert>

namespace hope::concurrency {

    template<typename T>
    class compound_queue final {
    public:

        compound_queue(compound_queue const&) = delete;
        compound_queue& operator = (compound_queue const&) = delete;

        compound_queue(compound_queue&& queue) = default;
        compound_queue& operator=(compound_queue&& queue) = default;

        explicit compound_queue()
            : m_owner_thread_id(std::this_thread::get_id()) {
            
        }

        ~compound_queue() {
            for(auto&& [_, queue] : m_queues) {
                delete queue;
            }
        }

        bool register_thread(const std::thread::id& id) {
            assert(m_owner_thread_id == id);
            if(!m_queues.count(id)) {
                m_queues.emplace(id, new spsc_queue<T>());
            }

            return true;
        }

        template<typename... Args>
        void enqueue(Args&&... v) {
            const std::thread::id& id = std::this_thread::get_id();
            assert(m_queues.count(id));
            m_queues[id]->enqueue(std::forward<Args>(v)...);
        }

        bool try_dequeue(T& v) {
            for(auto&& [_, q] : m_queues) {
                if(q->try_dequeue(v)) {
                    return true;
                }
            }

            return false;
        }

    private:
        const std::thread::id m_owner_thread_id;
        std::unordered_map<std::thread::id, spsc_queue<T>*> m_queues;
    };

}
