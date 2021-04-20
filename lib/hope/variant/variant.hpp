/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/components/unique_types.h"
#include "hope/variant/variant_storage.h"

namespace hope {

    template <typename... Ts>
    class variant final
    {
    public:
        variant() = default;

        template <typename T>
        explicit variant(T&& type);

        template <typename T>
        variant& operator=(T&& value);

        template<typename T>
        T& get();
    private:
        constexpr static unique_types<Ts...>     UniqueChecker{ };
        detail::variant_storage<Ts...>           m_storage;
    };
}
