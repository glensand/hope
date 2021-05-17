/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/components/policy_map/policy_map.h"
#include <iostream>

namespace hope::sample {

    class map_policy final
    {
		struct base { };

		struct use_inverter final : base { };
		struct use_converter final : base { };
		struct use_default final : base { };

		struct use_reference final : base { };
		struct use_value final : base { };

		struct const_q final : base { };
		struct non_const_q final : base { };

		template<typename... Ps>
		using test_map = hope::policy::map<
			hope::policy::variant_list<
				hope::policy::variant<use_converter, use_inverter, use_default>,
				hope::policy::variant<use_reference, use_value>,
				hope::policy::variant<const_q, non_const_q>
			>,
			base,
			Ps...
		>;

    public:
		map_policy() = default;
        ~map_policy() = default;

		static void run() {
			do_smth<use_inverter, use_value, const_q>();
			do_smth<use_converter, use_reference, const_q>();
		}

    private:

		template<typename... Ps>
		static void do_smth() {
			using map = test_map<Ps...>;
			if constexpr (map::template has<use_inverter>()) {
				std::cout << "use_inverter ";
			}
				
			if constexpr (map::template has<use_converter>()) {
				std::cout << "use_converter ";
			}

			if constexpr (map::template has<use_reference>())
				std::cout << "use_reference ";

			std::cout << std::endl;
		}
    };
}
