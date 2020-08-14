/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

namespace hope::memory::config{
    constexpr size_t MaxObjectSize{ 64 };
    constexpr size_t ChunkSize{ 255 };
    constexpr bool TestMode{ true };
}