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