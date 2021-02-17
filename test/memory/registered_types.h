/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <gtest/gtest.h>
#include "objects_aligned_to8.h"
#include "objects_aligned_to4.h"
#include "objects_aligned_to1.h"


using TestTypes = testing::Types<
    RegisteredTypesAscendingAlignedTo1,
    RegisteredTypesAscendingAlignedTo4,
    RegisteredTypesAscendingAlignedTo8,
    RegisteredTypesDescendingAlignedTo1,
    RegisteredTypesDescendingAlignedTo4,
    RegisteredTypesDescendingAlignedTo8,
    RegisteredTypesShuffledAlignedTo1,
    RegisteredTypesShuffledAlignedTo4,
    RegisteredTypesShuffledAlignedTo8
>;