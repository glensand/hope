/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <type_traits>
#include <cassert>
#include "hope/typelist/type_list.h"

namespace hope {

    namespace detail {
        template <typename... Ts>
        class variant_storage final {
        public:
            template<typename T>
            T& get() {
                check_conditions<T>();
                return reinterpret_cast<T&>(m_storage);
            }

            template<typename T>
            [[nodiscard]] const T& get() const {
                check_conditions<T>();
                return reinterpret_cast<const T&>(m_storage);
            }

            template<typename T, typename... Vs>
            void initialize(Vs&&... values) {
                static_assert(contains<T>(Types));
                delete_cur_value();
                update_type_index<T>();
                new(&m_storage) T(std::forward<Vs>(values)...);
            }

            template<typename T>
            void set(T&& value) {
                if (m_cur_type_index == find<T>(type_list<Ts...>{}))
                    get_impl<T>() = std::forward<T>(value);
                else 
                    initialize<T>(std::forward<T>(value));
            }

        private:

            template <typename T>
            T& get_impl() {
                return reinterpret_cast<T&>(m_storage);
            }

            template<typename T>
            void check_conditions() {
                static_assert(contains<T>(Types));
                assert(is_initialized());
            }

            void delete_cur_value() {
                if (is_initialized()) {
                    // it seems like vs142 still has some bugs...
                    // im sure it should works fine
                    // (DeactivateImpl<Ts>, ...);

                    // Old style curly braces
                    const bool bs[] = { deactivate_impl<Ts>()... };
                    (void)bs;
                }
            }

            template <typename T>
            constexpr bool deactivate_impl() {
                if (m_cur_type_index == find<T>(Types)) {
                    reinterpret_cast<T*>(&m_storage)->~T();
                    m_cur_type_index = TypesCount;
                }
                return { };
            }

            template <typename T>
            constexpr void update_type_index() {
                m_cur_type_index = find<T>(Types);
            }

            [[nodiscard]] bool is_initialized() const {
                return m_cur_type_index < TypesCount;
            }

            constexpr static type_list<Ts...>   Types{ };
            constexpr static size_t             LargestTypeIdx{ largest_type_index(type_list<Ts...>{}) };
            constexpr static size_t             TypesCount = size(Types);

            using largest_t = typename decltype(largest_type(type_list<Ts...>{}))::Type;
            using storage_t = std::aligned_storage_t<sizeof(largest_t), alignof(largest_t)>;

            storage_t       m_storage{ };
            std::size_t     m_cur_type_index{ size(type_list<Ts...>{}) };
        };
    }
}