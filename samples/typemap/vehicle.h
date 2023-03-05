/* Copyright (C) 2021 - 2023 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/components/singleton_holder/singleton_holder.h"
#include "hope/components/link_holder/link_holder_array.h"

#include <cassert>

#include "vehicle_modules.h"

namespace hope::sample::typemap {

    class vehicle final {
    public:
        vehicle() = default;
    
        void add_module(vehicle_module* link) {
            (void)m_modules.add(link);
        }

        void assert_vehicle_valid() const noexcept{
            for (auto* link : m_modules.get_links())
            {
                assert(link != nullptr);
                (void)link; // remove clang unused variable warning
            }
                
        }

    private:
        using module_set_t = link_holder_array<
            vehicle_module,
            graphics,
            logic,
            physics
        >;

        module_set_t m_modules{ };
    };
}
