//------------------------------------------------------------------------------
// fill.h
//
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 27.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include <array>

// TODO:: implement constexpr iterator based filling
namespace hope{

    template<typename T, std::size_t S1, std::size_t S2>
    constexpr void* fill(const std::array<T, S1>& src, std::array<T, S2>& dst){
        
    }
}
