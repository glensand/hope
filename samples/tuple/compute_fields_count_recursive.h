/* Copyright (C) 2021 - 2023 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/tuple/tuple_from_struct.h"
#include "hope/tuple/compute_field_count_recursive.h"
#include "recursive_structure.h"

#include <iostream>

namespace hope::sample::compute_fields_count_recursive {

    template<typename T>
    constexpr std::enable_if_t<!std::is_class_v<T>, std::size_t>
    compute(T) {
        return 1;
    }

    template<typename T>
    constexpr std::enable_if_t<std::is_class_v<T>, std::size_t>
    compute(T) {
        constexpr auto tuple = tuple_from_struct(T{}, field_policy::value{});
        std::size_t count{ 0 };
        for_each(tuple, [&](auto field) {
            count += compute(field);
        });
        return count;
    }

    inline
    void run() {
        recursive_structure s{1, 1.f,{511, 2354, { -111, true}}};

        std::cout << compute(s) << std::endl;
        std::cout << compute_field_count_recursive_constexpr<recursive_structure>() << std::endl;
    }
}
