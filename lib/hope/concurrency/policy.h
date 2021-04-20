/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <mutex>

namespace hope::concurrency {

    template <typename Policy>
    class threading_policy {
    public:
        void lock() {
            m_threading_policy.lock();
        }

        void unlock() {
            m_threading_policy.unlock();
        }
    private:
        Policy m_threading_policy;
    };

    namespace policy {

        class single_threaded final {
        public:
            /**
             * \brief default constructor cannot be constexpr, but according to standard it have to be
             */
            constexpr single_threaded() {}
            ~single_threaded() = default;

            /**
             * \brief
             */
            void lock() {}

            /**
             * \brief
             */
            void unlock() {}
        };

        class multi_threaded final {
        public:
            /**
             * \brief
             */
            constexpr multi_threaded() {}
            ~multi_threaded() = default;

            /**
            * \brief
            */
            void lock() {
                m_mutex.lock();
            }

            /**
             * \brief
             */
            void unlock() {
                m_mutex.unlock();
            }

        private:
            std::mutex m_mutex;
        };

    }
}