/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "components/singleton_holder/singleton_holder.h"
#include <iostream>

namespace {
	
	struct simple_runner_impl final {
		void run() {
			std::cout << "simple_runner::run()" << std::endl;
		}
	};

	using simple_runner = hope::singleton_holder<simple_runner_impl>;
}

int main()
{
	simple_runner::instance().run();

	return 0;
}