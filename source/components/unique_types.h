/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <type_traits>

template <typename ... Ts>
struct unique_types;

template <typename T1, typename T2, typename ... Ts>
struct unique_types<T1, T2, Ts ...>
    : unique_types<T1, T2>, unique_types<T1, Ts ...>, unique_types<T2, Ts ...> {};

template <typename T1, typename T2>
struct unique_types<T1, T2> {
    static_assert(!std::is_same_v<T1, T2>, "Types must be unique.");
};