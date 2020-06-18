//------------------------------------------------------------------------------
// variant.hpp
//
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 13.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "core/UniqueTypes.h"
#include "variant/VariantStorage.h"

template <typename... Ts>
class Variant final
{
public:
    Variant() = default;

    template <typename T>
    explicit Variant(T&& type);

    template <typename T>
    Variant& operator=(T&& value);

    template<typename T>
    T& Get();

private:
    constexpr static UniqueTypes<Ts...>     UniqueChecker{ };
    Detail::VariantStorage<Ts...>           m_storage;
};