/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "typelist/type_list.h"
#include <array>

namespace hope {

    template<typename BaseType, typename... Links>
    class link_holder final {
    public:
        
        using link_list = std::array<BaseType*, size(type_list<Links...>{})> ;

        link_holder() {
            for (auto& link : links)
                link = nullptr;
        }

        ~link_holder() = default;

        template <typename T>
        constexpr T* get() const noexcept { 
            return get_impl<T>();
        }

        bool add_link(BaseType* link) noexcept {
            const auto index = find_index(link);
            if (index < holder_size) {
                if (links[index] == nullptr) {
                    links[index] = link;
                    return true;
                }
            }
            return false;
        }

        bool remove_link(BaseType* link) noexcept {
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

        const link_list& get_links() const noexcept {
            return links;
        }

        link_holder(const link_holder&) = delete;
        link_holder(link_holder&&) = delete;
        link_holder& operator=(const link_holder&) = delete;
        link_holder& operator=(link_holder&&) = delete;
    private:
    
        template <typename T, typename NativeT = std::decay_t<T>>
        constexpr T* get_impl() const noexcept {
            static_assert(contains<NativeT>(types));
            constexpr std::size_t Index = find<NativeT>(types);
            return static_cast<T*>(links[Index]);
        }

        std::size_t find_index(BaseType* link) noexcept {
            constexpr auto seq = std::make_index_sequence<holder_size>();
            return try_cast(link, seq);
        }

        template <typename T, typename NativeT = std::decay_t<T>, std::size_t... Is>
        std::size_t try_cast(T* link, std::index_sequence<Is...> seq) noexcept {
            return find_if(types, [&](auto holder) {
                return dynamic_cast<typename decltype(holder)::Type*>
                    (link) != nullptr;
                });
        }

        constexpr static type_list<Links...> types{ };
        constexpr static std::size_t holder_size{ size(types) };

        link_list links{ };
    };
}