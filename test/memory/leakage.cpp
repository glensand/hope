/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"
#include <vector>
#include <random>

#include "sm_objects.h"
#include "objects_aligned_to1.h"
#include "objects_aligned_to4.h"
#include "objects_aligned_to16.h"

namespace hope::memory::testing{

	template <typename RegisteredTypes>
	class leakage_test : public ::testing::Test {
	protected:
		void SetUp() override {
			_CrtMemCheckpoint(&m_startup);
		}

		void TearDown() override {
			_CrtMemState teardown, diff;
			_CrtMemCheckpoint(&teardown);
			ASSERT_EQ(0, _CrtMemDifference(&diff, &m_startup, &teardown)) << "Memory leaks detected";
		}
		_CrtMemState m_startup{ };
	};

	using TestTypes = ::testing::Types
		<
		RegisteredTypesAscendingAlignedTo1,
		RegisteredTypesAscendingAlignedTo4,
		RegisteredTypesAscendingAlignedTo16,
		RegisteredTypesDescendingAlignedTo1,
		RegisteredTypesDescendingAlignedTo4,
		RegisteredTypesDescendingAlignedTo16,
		RegisteredTypesShuffledAlignedTo1,
		RegisteredTypesShuffledAlignedTo4,
		RegisteredTypesShuffledAlignedTo16
		>;

	constexpr std::size_t ObjectsCount{ 300u };

	template <typename... Ts>
	void apply_alloc_dealloc(type_list<Ts...> list) {
		alloc_dealloc(list, ObjectsCount, [](auto&&) {});
		alloc_dealloc(list, ObjectsCount,
			[](auto&& sm_list) {
				auto rng = std::default_random_engine{};
				std::shuffle(std::begin(sm_list), std::end(sm_list), rng);
			});
	}

	TYPED_TEST_CASE(leakage_test, TestTypes);

	TYPED_TEST(leakage_test, sm_alLOCATOR_LEAKAGE)
	{
		apply_alloc_dealloc(TypeParam{});
	}

}
