/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 *
 * The main idea of the utilities which are located here is honestly borrowed by Björn Fahller report form cpp Piter 2019 conference
 */

#pragma once

#include <type_traits>
#include <string>
#include <vector>
#include <array>

namespace hope {

    template <typename T>
    struct is_string final : std::false_type {};

    template <class T, class Traits, class Alloc>
    struct is_string<std::basic_string<T, Traits, Alloc>> final : std::true_type {};

    template<typename T>
    constexpr static bool is_string_v = is_string<T>::value;

    template <typename T>
    struct is_vector final : std::false_type {};

    template <class T, class Alloc>
    struct is_vector<std::vector<T, Alloc>> final : std::true_type {};

    template<typename T>
    constexpr static bool is_vector_v = is_vector<T>::value;

    template<typename T>
    struct is_array final : std::false_type{};

    template<typename T, std::size_t S>
    struct is_array<std::array<T, S>> final : std::true_type {};

    template<typename T>
    constexpr static bool is_array_v = is_array<T>::value;

    template<typename T>
    constexpr static bool is_user_defined_type_v =
        std::is_class_v<T> &&
        !is_string_v<T> &&
        !is_vector_v<T> && 
        !is_array_v<T>;

}