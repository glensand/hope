/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <vector>
#include <iostream>

#include "hope/tuple/flat_tuple.h"

namespace hope::sample::tuple_for_each {

    inline
    void simple_type_print() {

        // create small tuple, constexpr cv may be removed, it is no'r nessesery
        constexpr auto tuple = make_flat_tuple("hello word...", 42, 1001.0);
        
        std::vector<std::string> names; // names will be filled with tuple's types obtained from typeid
        
        for_each(tuple, [&](auto&& val) {
            using clean_t = std::decay_t<decltype(val)>;
            names.emplace_back(typeid(clean_t).name());
            });

        for (auto&& name : names)
            std::cout << name << std::endl;
    }

}