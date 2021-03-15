/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once 

#include "components/singleton_holder/singleton_holder.h"
#include "components/factory.h"

namespace hope::sample::object_factory {
	
	class object {
	public:
		virtual ~object() = default;
		virtual std::string_view name() const noexcept = 0;
	};

	using object_factory = singleton_holder<factory<object>>;

#define REGISTER_OBJECT(ClassName) static const auto registered_##ClassName = object_factory::instance().register_object<ClassName>(#ClassName);
#define DECLARE_OBJECT(ClassName) class ClassName final : public object { \
		virtual std::string_view name() const noexcept { return #ClassName; } \
	};\

DECLARE_OBJECT(simple_runner)
REGISTER_OBJECT(simple_runner)

DECLARE_OBJECT(top_runner)
REGISTER_OBJECT(top_runner)

DECLARE_OBJECT(hope_runner)
REGISTER_OBJECT(hope_runner)

}
