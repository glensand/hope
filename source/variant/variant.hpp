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

#include "core/unique_types.h"
#include "variant/variant_storage.h"

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
