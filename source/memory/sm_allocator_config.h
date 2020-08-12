//------------------------------------------------------------------------------
// sm_allocator_config.h
// 
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 13.08.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

namespace hope::memory::config{
    constexpr std::size_t MaxObjectSize{ 64 };
    constexpr std::size_t ChunkSize{ 255 };
    constexpr bool TestMode{ true };
}