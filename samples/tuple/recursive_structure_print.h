/* Copyright (C) 2021 - 2023 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/tuple/tuple_from_struct.h"
#include "recursive_structure.h"
#include <iostream>

namespace hope::sample::recursive_structure_print {

    template<typename T>
    std::enable_if_t<!std::is_class_v<T>, void>
    print(const T& object, std::size_t tabs = 0) {
        for (std::size_t i{ 0 }; i < tabs; ++i)
            std::cout << "  ";
        std::cout << object << std::endl;
    }

    template<typename T>
    std::enable_if_t<std::is_class_v<T>, void>
    print(const T& object, int tabs = -1) {
        auto&& tuple = tuple_from_struct(object, field_policy::reference{});
        for_each(tuple, [=](auto&& field) {
            print(field, tabs + 1);
        });
    }

    inline
    void run() {
        recursive_structure s{1, 1.f,{511, 2354, { -111, true}}};

        print(s);
    }
}
