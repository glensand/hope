//------------------------------------------------------------------------------
// variant.hpp
//
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 09.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "variant/variant.hpp"

template <typename ... Ts>
template <typename T>
Variant<Ts...>::Variant(T&& type)
{
    static_assert(Contains<T>(TypeList<Ts...>{}));
    m_storage.template Initialize<T>(std::forward<T>(type));
}

template <typename ... Ts>
template <typename T>
Variant<Ts...>& Variant<Ts...>::operator=(T&& value)
{
    m_storage.Set(std::forward<T>(value));
    return *this;
}

template <typename ... Ts>
template <typename T>
T& Variant<Ts...>::Get()
{
    return m_storage.template Get<T>();
}
