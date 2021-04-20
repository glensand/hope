/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/variant/variant.hpp"

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
