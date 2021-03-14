/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "components/typemap.h"
#include "components/singleton_holder/singleton_holder.h"
#include "components/link_holder.h"

#include <cassert>

#include "vehicle_modules.h"

namespace hope::sample::typemap {

    class vehicle final {
    public:
        vehicle() = default;
    
        void add_module(vehicle_module* link) {
            (void)m_modules.add_link(link);
        }

        void assert_vehicle_valid() const noexcept{
            for (auto* link : m_modules.get_links())
                assert(link != nullptr);
        }

    private:
        using module_set_t = hope::link_holder<
            hope::sample::typemap::vehicle_module,
            hope::sample::typemap::graphics,
            hope::sample::typemap::logic,
            hope::sample::typemap::physics
        >;

        module_set_t m_modules{ };
    };
}
