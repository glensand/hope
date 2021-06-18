/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
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

    class single_value final
    {
		template <typename... Ts>
		using link_holder_tuple = link_holder_tuple<link_holder_policy::single_value, Ts...>;

		using holder = link_holder_tuple<
			derived1,
			derived2,
			derived3,
			derived4,
			derived5,
			derived6
		>;

    public:
        single_value() = default;
        ~single_value() = default;

		static void run() {

			holder h;
			base* d1 = new derived1;
			base* d2 = new derived2;
			base* d5 = new derived5;
			base* d7 = new derived7;

			bool add_res1 = h.add(d1);
			bool add_res2 = h.add(d2);
			bool add_res3 = h.add(d5);
			bool add_res4 = h.add(d7);
			assert(add_res4 == false);
			assert(add_res1 == add_res2 == add_res3 == true);

			auto* p = h.get<derived1>();

			std::cout << h.get<derived1>()->name() << std::endl
				<< h.get<derived2>()->name() << std::endl
				<< h.get<derived5>()->name() << std::endl;

			bool remove_res1 = h.remove(d1);
			bool remove_res2 = h.remove(d2);
			bool remove_res3 = h.remove(d5);
			assert(remove_res1 == remove_res2 == remove_res3 == true);

			assert(h.get<derived1>() == nullptr);
			assert(h.get<derived2>() == nullptr);
			assert(h.get<derived5>() == nullptr);
		}
    };
}
