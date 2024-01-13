/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include <vector>
#include <random>
#include "registered_types.h"
#include "sm_objects.h"

namespace hope::memory::testing{

    template <typename RegisteredTypes>
    class leakage_test : public ::testing::Test {
    protected:
#if defined(_WIN32) || defined(_WIN64)
        void SetUp() override {
            small_object_allocator::instance();
            _CrtMemCheckpoint(&m_startup);
        }

        void TearDown() override {
            _CrtMemState teardown{}, diff{};
            _CrtMemCheckpoint(&teardown);
            ASSERT_EQ(0, _CrtMemDifference(&diff, &m_startup, &teardown)) << "Memory leaks detected";
            (void)teardown;
            (void)diff;
        }
        _CrtMemState m_startup{ };
#endif
    };

    constexpr std::size_t ObjectsCount{ 300u };

    TYPED_TEST_SUITE(leakage_test, TestTypes);

//    TYPED_TEST(leakage_test, sm_allocator_leakage_simple)
//    {
//        apply_alloc_dealloc<simple_sm_object>(TypeParam{}, ObjectsCount);
//        sm_allocator_reset::apply();
//    }
//
//    TYPED_TEST(leakage_test, sm_allocator_leakage_complicated)
//    {
//        apply_alloc_dealloc<complicated_sm_object>(TypeParam{}, ObjectsCount);
//        sm_allocator_reset::apply();
//    }

}
