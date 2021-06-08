/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <string>
#include <iostream>

#include "hope/tuple/tuple_from_struct.h"

namespace hope::sample::not_pod_to_tuple {

    struct not_pod final {
        std::string str;
        float f;
        bool b;
        double d;
    };

    inline
    void run() {

        not_pod s{"string", 1.f};

        auto&& tuple = tuple_from_struct(s);
        for_each(tuple, [](auto&& field)
        {
            std::cout << field;
        });
    }

}