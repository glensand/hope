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

    template <typename T>
    class sutter_queue final {

        struct node final {
            template<typename TVal>
            explicit node(TVal&& val)
                : value(std::forward<TVal>(val)), next(nullptr) { }

            T value;

            node* next{ nullptr };
        };
    
    public:

        // This queue must be fully constructed before being used in another thread.
        sutter_queue() {
            m_first = m_divider = m_last = new node(T());
        }
    
        ~sutter_queue() {
            while (m_first != nullptr) {
                node* temp = m_first;
                m_first = temp->next;
                delete temp;
            }
        }
    
        template<typename TVal>
        void enqueue(TVal&& item) {
            auto* last = m_last.load();
            last->next = new node(std::forward<TVal>(item));
    
            produce_impl(last);
        }
    
        bool dequeue(T& item) {
            auto* divNode = m_divider.load();
            if (auto * lastNode = m_last.load(); divNode != lastNode)
            {
                item = std::move(divNode->next->value);
                divNode = divNode->next;
                m_divider.store(divNode);
    
                return true;
            }
            return false;
        }
    
    private:
        void produce_impl(node* tmpLast) {
            tmpLast = tmpLast->next;
    
            m_last.store(tmpLast);
    
            node* div = m_divider.load();
            while (m_first != div) {
                node* temp = m_first;
                m_first = m_first->next;
                delete temp;
    
                div = m_divider.load();
            }
        }

        constexpr static std::size_t CacheLineSize{ 64 };

        node* m_first;    // Producer Only

        uint8_t m_cache_padding1[CacheLineSize]{ };

        std::atomic<node*>     m_divider;    // Producer / Consumer

        uint8_t m_cache_padding2[CacheLineSize]{ };

        std::atomic<node*>     m_last;        // Producer / Consumer
    };
}