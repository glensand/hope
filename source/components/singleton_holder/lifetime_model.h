/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <stdexcept>

namespace hope {

    namespace detail {
        constexpr char* OnErrorMessage{ "An attempt was made to get invalid singleton instance, class : " };
        using Deleter = void();
    }

    template <typename SingletonImpl>
    struct lifetime_auto final {
        static void on_dead_reference() {
            throw std::runtime_error(
                std::string(detail::OnErrorMessage) + typeid(SingletonImpl).name());
        }

        static void register_deleter(detail::Deleter deleter) {
            atexit(deleter);
        }
    };

    template <typename SingletonImpl>
    struct phoenix final {
        static void on_dead_reference() {
            // ignore 
        }

        static void register_deleter(detail::Deleter deleter) {
            atexit(deleter);
        }
    };

    template <typename SingletonImpl>
    struct immortal final {
        static void on_dead_reference() {
            throw std::runtime_error(
                std::string(detail::OnErrorMessage) + typeid(SingletonImpl).name());
        }

        static void register_deleter(detail::Deleter deleter) {
            // ignore
        }
    };

    template <typename SingletonImpl>
    struct life_controller final {
        static void on_dead_reference() {
            throw std::runtime_error(
                std::string(detail::OnErrorMessage) + typeid(SingletonImpl).name());
        }

        static void register_deleter(detail::Deleter deleter) {
            // ignore
        }
    };

}
