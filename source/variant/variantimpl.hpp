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

namespace hope{

    template <typename ... Ts>
    template <typename T>
    variant<Ts...>::variant(T&& type)
    {
        static_assert(contains<T>(type_list<Ts...>{}));
        m_storage.template initialize<T>(std::forward<T>(type));
    }

    template <typename ... Ts>
    template <typename T>
    variant<Ts...>& variant<Ts...>::operator=(T&& value)
    {
        m_storage.set(std::forward<T>(value));
        return *this;
    }

    template <typename ... Ts>
    template <typename T>
    T& variant<Ts...>::get()
    {
        return m_storage.template get<T>();
    }

}
