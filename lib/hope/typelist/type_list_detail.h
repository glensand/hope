/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <utility>
#include <cstddef> // NOTE: do not remove dis header, it is required for successful g++ compilation (size_t definition) 

namespace hope::detail {

    template <typename Is>
    struct get;

    template <size_t... Is>
    struct get<std::index_sequence<Is...>> {
        template <typename T>
        static constexpr T extractor(decltype(Is, static_cast<void*>(0))..., T*, ...);
    };

}