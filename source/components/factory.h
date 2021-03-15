/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "componens/loophole.h"

namespace hope {

    template <typename FactoryImpl>
    class factory {
        template <const char* Name, typename Type>
        struct typename_holder final {

        };
    public:
        factory() = default;
        ~factory() = default;

        template <const char* Name, typename Type>
        constexpr static bool register_type() noexcept {
            loophole::inject(typename_holder<Name, FactoryImpl>{ }, Type{ });
        }

        auto create(const char* name) const {
            using object_t = decltype(loophole::get());
        }

    private:

    };
}