/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <array>

// TODO:: implement constexpr iterator based filling
namespace hope{

    template<typename T, std::size_t S1, std::size_t S2>
    constexpr void* fill(const std::array<T, S1>& src, std::array<T, S2>& dst){
        
    }
}
