//------------------------------------------------------------------------------
// fastpimpl.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 22.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "typelist/TypeList.h"
#include <memory>

namespace Detail
{
 
}

template <typename T, std::size_t Size, std::size_t Align>
class FastPImpl final
{
    using StorageT = std::aligned_storage_t<Size, Align>;
public:

    ~FastPImpl()
    {
        CheckSizeAlign();
        reinterpret_cast<T&>(m_storage).~T();
    }

    template<typename... Ts>
    FastPImpl(Ts&&... args)
    {
        static_assert(std::is_constructible_v<T, Ts...>);
        new(&m_storage) T(std::forward<Ts>(args)...);
    }

    T* operator->() noexcept
    {
        return &m_ref;
    }

    const T* operator->() const noexcept
    {
        return &m_ref;
    }

    T& operator=(const T& rhs) noexcept
    {
        m_ref = rhs;
        return *this;
    }

    T& operator*() noexcept
    {
        return m_ref;
    }

    const T& operator*() const noexcept
    {
        return m_ref;
    }

private:

    static void CheckSizeAlign()
    {
        constexpr auto align = alignof(T);
        constexpr auto size = sizeof(T);

        static_assert(Align == align/*, "Correct align is: "*/);
        static_assert(Size == size/*, "Correct align is: "*/);
    }

    StorageT    m_storage;
    T&          m_ref{ reinterpret_cast<T&>(m_storage) };
};