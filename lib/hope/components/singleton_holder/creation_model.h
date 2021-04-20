/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <utility>

namespace hope {

    template <typename SingletonImpl>
    struct create_via_new final {
        static SingletonImpl* create() {
            return new SingletonImpl;
        }

        static void destroy(SingletonImpl* instance) {
            delete instance;
        }
    };

    template <typename SingletonImpl>
    struct create_static final {
        static SingletonImpl* create() {
            static std::aligned_storage_t<
                sizeof(SingletonImpl),
                alignof(SingletonImpl)
            > storage;
            auto* instance = new (&storage) SingletonImpl;
            return instance;
        }

        static void destroy(SingletonImpl* instance) {
            instance->~SingletonImpl();
        }
    };

}
