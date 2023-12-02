/* Copyright (C) 2021 - 2023 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"
#include "hope/components/switch_expression/switch_expression.h"
#include <iostream>

TEST(SwitchExpressionTest, Initialize)
{
    hope::switch_expression_lambda(
        "key1", [] { std::cout << "key1"; },
        "key2", [] { std::cout << "key2"; },
        "key3", [] { std::cout << "key3"; },
        "key4", [] { std::cout << "key4"; }
    ).apply("key1");
}