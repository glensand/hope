/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "hope/components/singleton_holder/singleton_holder.h"
#include "object_factory.h"
#include <cassert>

namespace {
	void assert_runner_valid(std::string_view name) {
		auto* instance = hope::sample::object_factory::object_factory::instance().create(name);
		assert(instance->name() == name);
		(void)instance;
	}
}

int main()
{
	assert_runner_valid("simple_runner");
	assert_runner_valid("hope_runner");
	assert_runner_valid("top_runner");

	return 0;
}