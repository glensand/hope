/* Copyright (C) 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <atomic>

namespace tsan{

    using thread_id = std::atomic<std::thread::id>;

    struct simple_tsan_scope final {
        simple_tsan_scope(thread_id& id)
            : this_id(id)
            , slave_scope(false) {
            if(id != std::this_thread::get_id()) {
                assert(id == std::thread::id{});
                id.store(std::this_thread::get_id(), std::memory_order_release);
            } else {
                slave_scope = true;
            }
        }

        ~simple_tsan_scope() {
            if(!slave_scope) {
                assert(std::this_thread::get_id() == this_id.load(std::memory_order_consume));
                this_id = std::thread::id{};
            }
        }

        thread_id& this_id;
        bool slave_scope;
    };
}

#define TSAN_SAFE static tsan::thread_id __tsan_mark__{ std::thread::id{} }; \
    tsan::simple_tsan_scope __tsan_scope_{ __tsan_mark__ };