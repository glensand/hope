/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include <iostream>

#include "memory/small_object.h"
#include <algorithm>
#include <random>

struct small_struct1 : hope::memory::small_object
{
	bool small1;
};

struct small_struct2 : hope::memory::small_object
{
	int a;
};

int main()
{
	std::vector<hope::memory::small_object*> vec;
	for(std::size_t i{ 0 }; i < 300; ++i) {
		vec.push_back(new small_struct1);
		vec.push_back(new small_struct2);
	}

	std::reverse(std::begin(vec), std::end(vec));

	for (const auto sm : vec)
		delete sm;
	return 0;
} 