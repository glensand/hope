/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <cassert>
#include <iostream>

#include "test_classes.h"
#include "hope/components/link_holder/link_holder_policy.h"
#include "hope/components/link_holder/link_holder_tuple.h"

namespace hope::sample {

    class multiple_value final
    {
        template <typename... Ts>
        using link_holder_tuple = link_holder_tuple<link_holder_policy::multiple_value, Ts...>;

        using holder = link_holder_tuple<
            derived1,
            derived2,
            derived3,
            derived5,
            derived6
        >;

    public:
        multiple_value() = default;
        ~multiple_value() = default;

        static void run() {
            just_add_check();
            add_remove_check();
        }

        static void just_add_check() {
            holder h;
            assert(h.add(new derived1) && h.add(new derived2) && h.add(new derived2));
            assert(h.add(new derived3) && h.add(new derived3) && h.add(new derived3));
            assert(!(h.add(new derived4) && h.add(new derived3) && h.add(new derived3)));
            std::cout << "multiple_value::just_add_check passed" << std::endl;
        }

        static void add_remove_check() {
            holder h;
            base* d1_0 = new derived1;
            base* d1_1 = new derived1;
            base* d1_2 = new derived1;
            (void)d1_0;
            (void)d1_1;
            (void)d1_2;
            assert(h.add(d1_0) && h.add(d1_1) && h.add(d1_2));
            assert(h.get<derived1>()[0] == d1_0);
            assert(h.get<derived1>()[1] == d1_1);
            assert(h.get<derived1>()[2] == d1_2);

            assert(h.remove(d1_0) && h.remove(d1_1) && h.remove(d1_2));
            std::cout << "multiple_value::add_remove_check passed" << std::endl;
        }
    };
}
