/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "hope/components/link_holder_tuple.h"
#include "test_classes.h"
#include <iostream>
#include <cassert>

namespace {
	using holder = hope::link_holder_tuple<
		hope::sample::derived1,
		hope::sample::derived2,
		hope::sample::derived3,
		hope::sample::derived4,
		hope::sample::derived5,
		hope::sample::derived6
	>;
}

int main()
{
	holder h;
	hope::sample::base* d1 = new hope::sample::derived1;
	hope::sample::base* d2 = new hope::sample::derived2;
	hope::sample::base* d5 = new hope::sample::derived5;
	hope::sample::base* d7 = new hope::sample::derived7;

	bool add_res1 = h.add(d1);
	bool add_res2 = h.add(d2);
	bool add_res3 = h.add(d5);
	bool add_res4 = h.add(d7);
	assert(add_res4 == false);
	assert(add_res1 == add_res2 == add_res3 == true);

	std::cout << h.get<hope::sample::derived1>()->name() << std::endl
		<< h.get<hope::sample::derived2>()->name() << std::endl
		<< h.get<hope::sample::derived5>()->name() << std::endl;

	bool remove_res1 = h.remove(d1);
	bool remove_res2 = h.remove(d2);
	bool remove_res3 = h.remove(d5);
	assert(remove_res1 == remove_res2 == remove_res3 == true);

	assert(h.get<hope::sample::derived1>() == nullptr);
	assert(h.get<hope::sample::derived2>() == nullptr);
	assert(h.get<hope::sample::derived5>() == nullptr);

	return 0;
} 