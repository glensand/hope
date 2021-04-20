/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/typelist/type_list.h"
#include <array>

namespace hope {

    template<typename BaseType, typename... Links>
    class link_holder_array final {
    public:

        using link_list = std::array < BaseType*, size(type_list<Links...>{}) > ;

        link_holder_array() {
            for (auto& link : links)
                link = nullptr;
        }

        ~link_holder_array() = default;

        template <typename T>
        [[nodiscard]] constexpr T* get() const noexcept {
            return get_impl<T>();
        }

        bool add(BaseType* link) noexcept {
            const auto index = find_index(link);
            if (index < holder_size) {
                links[index] = link;
                return true;
            }
            return false;
        }

        bool remove(BaseType* link) noexcept {
            for (auto& link_ref : links) { // NOTE! we should not try to cast links one more time; it is assumed that the array contains only properly casted entities
                if (link == link_ref) {
                    link_ref = nullptr;
                    return true;
                }
            }
            return false;
        }

        link_list& get_links() noexcept {
            return links;
        }

        [[nodiscard]] const link_list& get_links() const noexcept {
            return links;
        }

        link_holder_array(const link_holder_array&) = delete;
        link_holder_array(link_holder_array&&) = delete;
        link_holder_array& operator=(const link_holder_array&) = delete;
        link_holder_array& operator=(link_holder_array&&) = delete;
    private:

        template <typename T, typename NativeT = std::decay_t<T>>
        [[nodiscard]] constexpr T* get_impl() const noexcept {
            static_assert(contains<NativeT>(types));
            constexpr std::size_t Index = find<NativeT>(types);
            if constexpr (std::is_convertible_v<BaseType*, NativeT*>)
                return static_cast<T*>(links[Index]);   // in case if NativeT inherit BaseType via virtual (just virtual inheritance) 
            else                                        // Base cannot be unambiguous converted to the derived
                return dynamic_cast<T*>(links[Index]);
        }

        std::size_t find_index(BaseType* link) noexcept {
            // ReSharper disable once CppEntityUsedOnlyInUnevaluatedContext
            return find_if(types, [&](auto&& holder) {
                using type = typename std::decay_t<decltype(holder)>::Type;
                constexpr std::size_t index = find<type>(types);
                return links[index] == nullptr && dynamic_cast<type*>(link) != nullptr;
                });
        }

        constexpr static type_list<Links...> types{ };
        constexpr static std::size_t holder_size{ size(types) };

        link_list links{ };
    };
}