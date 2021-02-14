//------------------------------------------------------------------------------
// ITest.h
/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once
#include "memory/small_object.h"

namespace hope::memory::testing {

	struct params {
		virtual ~params() = default;
	};

	/**
	* \brief this is really funny to use small object to perform small objects testing :)
	*/
	class test_base : small_object {
	public:
		test_base() = default;
		virtual ~test_base() = default;
	
		virtual void run(const params& test_params = params{}) const = 0;
	
	    [[nodiscard]] virtual const char* test_name() const = 0;
	};

}
