/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"
#include <random>

#include "sm_objects.h"
#include "registered_types.h"

namespace hope::memory::testing {

    template <typename RegisteredTypes>
    class stress_test : public ::testing::Test {
    protected:
        void SetUp() override {
            small_object_allocator::instance();
        }

        void TearDown() override {
        }
    };

    constexpr std::size_t ObjectsCount{ 3000u };

    TYPED_TEST_SUITE(stress_test, TestTypes);

//    TYPED_TEST(stress_test, sm_allocator_stress_simple)
//    {
//        apply_alloc_dealloc<simple_sm_object>(TypeParam{}, ObjectsCount);
//        apply_alloc_dealloc<simple_sm_object>(TypeParam{}, ObjectsCount);
//        apply_alloc_dealloc<simple_sm_object>(TypeParam{}, ObjectsCount);
//        apply_alloc_dealloc<simple_sm_object>(TypeParam{}, ObjectsCount);
//        sm_allocator_reset::apply();
//    }
//
//    TYPED_TEST(stress_test, sm_allocator_stress_complicated)
//    {
//        apply_alloc_dealloc<complicated_sm_object>(TypeParam{}, ObjectsCount);
//        apply_alloc_dealloc<complicated_sm_object>(TypeParam{}, ObjectsCount);
//        apply_alloc_dealloc<complicated_sm_object>(TypeParam{}, ObjectsCount);
//        apply_alloc_dealloc<complicated_sm_object>(TypeParam{}, ObjectsCount);
//        sm_allocator_reset::apply();
//    }

}
