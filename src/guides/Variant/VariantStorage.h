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
#include <cassert>
#include "typelist/TypeList.h"

namespace Detail
{

    template <typename... Ts>
    class VariantStorage final
    {
    public:

        template<typename T>
        T&  Get()
        {
            CheckConditions<T>();
            return reinterpret_cast<T&>(m_storage);
        }

        template<typename T>
        [[nodiscard]] const T& Get() const
        {
            CheckConditions<T>();
            return reinterpret_cast<const T&>(m_storage);
        }

        template<typename T, typename... Vs>
        void Initialize(Vs&&... values)
        {
            static_assert(Contains<T>(Types));
            DeleteCurValue();
            UpdateTypeIndex<T>();
            new(&m_storage) T(std::forward<Vs>(values)...);
        }

        template<typename T>
        void Set(T&& value)
        {
            if (m_curTypeIndex == Find<T>(TypeList<Ts...>{}))
            {
                GetImpl<T>() = std::forward<T>(value);
            }
            else
            {
                Initialize<T>(std::forward<T>(value));
            }
        }

    private:

        template <typename T>
        T& GetImpl()
        {
            return reinterpret_cast<T&>(m_storage);
        }

        template<typename T>
        void CheckConditions()
        {
            static_assert(Contains<T>(Types));
            assert(IsInitialized());
        }

        void DeleteCurValue()
        {
            if (IsInitialized())
            {
                // it seems like vs142 still has some bugs...
                // im sure it should works fine
                // (DeactivateImpl<Ts>, ...);

                // Old style curly braces
                const bool bs[] = { DeactivateImpl<Ts>()... };
                (void)bs;
            }
        }

        template <typename T>
        constexpr bool DeactivateImpl()
        {
            if(m_curTypeIndex == Find<T>(Types))
            {
                reinterpret_cast<T*>(&m_storage)->~T();
                m_curTypeIndex = TypesCount;
            }
            return { };
        }

        template <typename T>
        constexpr void UpdateTypeIndex()
        {
            m_curTypeIndex = Find<T>(Types);
        }

        [[nodiscard]] bool IsInitialized() const
        {
            return m_curTypeIndex < TypesCount;
        }

        constexpr static TypeList<Ts...>    Types{ };
        constexpr static size_t             LargestTypeIdx{ LargestTypeIndex(TypeList<Ts...>{}) };
        constexpr static size_t             TypesCount = Size(Types);

        using LargestType = typename decltype(LargestType(TypeList<Ts...>{}))::Type;
        using StorageType = std::aligned_storage_t<sizeof(LargestType), alignof(LargestType)>;

        StorageType     m_storage{ };
        size_t          m_curTypeIndex { Size(TypeList<Ts...>{}) };
    };

}
