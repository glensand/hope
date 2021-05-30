/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/tuple/tuple_from_struct.h"
#include <iostream>

namespace hope::sample::recursive_structure_print {

    struct recursive_structure final {

        int field0;
        float field2;

        struct inner_structure1 final {
            int field1;
            int field2;

            struct inner_structure2 final {
                int field1;
                bool field2;
            };

            inner_structure2 field3;
        };
        inner_structure1 field3;
    };

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
