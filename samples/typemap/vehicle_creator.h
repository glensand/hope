/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "vehicle.h"

namespace hope::sample::typemap {

    class vehicle_creator final {
    public:
        template <typename... Ts, typename Tag>
        static vehicle* create(type_map<Ts...> modules, Tag type_tag) {
            using map = type_map<Ts...>;
            using module_list = typename decltype(map::get<Tag>())::Type;
            auto* vehicle_instance = new vehicle;
            for_each(module_list{ }, [&] (auto type_tag) {
                vehicle_instance->add_module(new typename decltype(type_tag)::Type);
                });
            return vehicle_instance;
        }
    };

}
