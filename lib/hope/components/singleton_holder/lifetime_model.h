/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <stdexcept>
#include <typeinfo>

namespace hope {

    namespace detail {
        constexpr const char* OnErrorMessage{"An attempt was made to get invalid singleton instance, class : "};
        using Deleter = void();
    }

    template <typename SingletonImpl>
    class lifetime_auto final {
    public:
        static void on_dead_reference() {
            throw std::runtime_error(
                std::string(detail::OnErrorMessage) + typeid(SingletonImpl).name());
        }

        static void register_deleter(detail::Deleter deleter) {
            atexit(deleter);
        }
    };

    template <typename SingletonImpl>
    class phoenix final {
    public:
        static void on_dead_reference() {
            // ignore 
        }

        static void register_deleter(detail::Deleter deleter) {
            atexit(deleter);
        }
    };

    template <typename SingletonImpl>
    class immortal final {
    public:
        static void on_dead_reference() {
            throw std::runtime_error(
                std::string(detail::OnErrorMessage) + typeid(SingletonImpl).name());
        }

        static void register_deleter(detail::Deleter deleter) {
            // ignore
        }
    };

    template <typename SingletonImpl>
    class life_controller final {
    public:
        static void on_dead_reference() {
            throw std::runtime_error(
                std::string(detail::OnErrorMessage) + typeid(SingletonImpl).name());
        }

        static void register_deleter(detail::Deleter deleter) {
            // ignore
        }
    };

}
