/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"

#include "hope/function/function_traits.h"

template<typename T>
auto traits(T f) {
    return hope::function_traits<T>{};
}

TEST(FunctionTraits, VoidLambda)
{
    auto&& func = [] {};
    auto&& trait = traits(func);
    ASSERT_TRUE(trait.arity == 0);
}
