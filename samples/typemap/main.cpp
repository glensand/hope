/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include <string_view>
#include "components/typemap.h"
#include "vehicle.h"
#include "vehicle_creator.h"
#include "vehicle_modules.h"

namespace {

	struct legacy_tag final {};
	struct vehicle_tag final {};

	using vehicle_types_t = hope::type_map<
		hope::type_pair<legacy_tag, hope::type_list<
			hope::sample::typemap::legacy_graphics,
			hope::sample::typemap::legacy_logic,
			hope::sample::typemap::legacy_physics
		>>,
		hope::type_pair<vehicle_tag, hope::type_list<
			hope::sample::typemap::vehicle_graphics,
			hope::sample::typemap::vehicle_logic,
			hope::sample::typemap::vehicle_physics
		>>
	>;
}

int main()
{
	auto* legacy_vehicle_instance = hope::sample::typemap::vehicle_creator::create(vehicle_types_t{ }, legacy_tag{ });
	auto* vehicle_instance = hope::sample::typemap::vehicle_creator::create(vehicle_types_t{ }, vehicle_tag{ });

	legacy_vehicle_instance->assert_vehicle_valid();
	vehicle_instance->assert_vehicle_valid();

	return 0;
} 
