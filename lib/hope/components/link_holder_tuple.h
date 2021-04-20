/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/typelist/type_list.h"
#include "hope/tuple/flat_tuple.h"

namespace hope {

    template<typename... Links>
    class link_holder_tuple final {
    public:
        using links_t = flat_tuple<Links*...>;

        template <typename T>
        using native_t = std::remove_pointer_t<std::decay_t<T>>;

        link_holder_tuple() {
            for_each(links, [](auto&& field) {
                field = nullptr;
            });
        }

        ~link_holder_tuple() = default;

        template <typename T>
        [[nodiscard]] constexpr T* get() const noexcept {
            return get_impl<T>();
        }

        template <typename T>
        bool add(T* link) noexcept {
            if (link == nullptr)
                return false;

            bool added = false;
            for_each(links, [&](auto&& field) {
                if (added) return;
                using self_native_t = native_t<decltype(field)>;
                if (auto* self_link = dynamic_cast<self_native_t*>(link)) {
                    field = self_link;
                    added = true;
                }
            });
            return added;
        }

        template <typename T>
        bool remove(T* link) noexcept {
            if (link == nullptr) 
                return false;

            bool removed = false;
            for_each(links, [&](auto&& field) {
                if (removed) return;
                using self_native_t = native_t<decltype(field)>;
                if (auto* self_link = dynamic_cast<self_native_t*>(link)) {
                    if(field == self_link) {
                        field = nullptr;
                        removed = true;
                    }
                }
            });
            return removed;
        }

        [[nodiscard]] links_t& get_links() noexcept {
            return links;
        }

        [[nodiscard]] const links_t& get_links() const noexcept {
            return links;
        }

        link_holder_tuple(const link_holder_tuple&) = delete;
        link_holder_tuple(link_holder_tuple&&) = delete;
        link_holder_tuple& operator=(const link_holder_tuple&) = delete;
        link_holder_tuple& operator=(link_holder_tuple&&) = delete;
    private:

        template <typename T, typename NativeT = std::decay_t<T>>
        [[nodiscard]] constexpr T* get_impl() const noexcept {
            static_assert(contains<NativeT>(types));
            return links.template get<NativeT*>();
        }

        constexpr static type_list<Links...> types{ };

        links_t links{ };
    };
}