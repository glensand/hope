//------------------------------------------------------------------------------
// Variant.h
//
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 09.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include <type_traits>
#include "CustomTraits/UniqueTypes.h"
#include "TypeList/TypeList.h"

template <typename... Ts>
class Variant
{
public:
    Variant() = default;

    template <typename T, typename = std::enable_if_t<Contains<T>(TypeList<Ts...>{})>>
    Variant(T&& type)
    {
        
    }

private:

    constexpr static size_t LargestTypeIdx = LargestType(TypeList<Ts...>{});
    using LargestType = decltype(GetNthType<LargestTypeIdx>(TypeList<Ts...>{}))::Type;

    std::aligned_storage<sizeof(LargestType), alignof(LargestType)> m_storage;

    UniqueTypes m_uniqueChecker;
};