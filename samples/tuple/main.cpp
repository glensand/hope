/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "simple_for_each.h"
#include "arguments.h"

template <typename F>
void run_impl(const char* func_name, F&& func) {
	std::cout << func_name << std::endl;
	func();
	std::cout << "//--------------------------------------------------//" << std::endl;
}

#define RUN(Name) run_impl(#Name, Name);  

int main() {
	RUN(hope::sample::tuple_for_each::simple_type_print)

	RUN(hope::sample::struct_serializer::run)

	return 0;
} 