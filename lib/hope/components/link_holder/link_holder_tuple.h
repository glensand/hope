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

    template<
        template<typename> typename InnerHolderPolicy,
        typename... Links
    >
    class link_holder_tuple final {
    public:
        using links_t = flat_tuple<InnerHolderPolicy<Links>...>;

        link_holder_tuple() = default;
        ~link_holder_tuple() = default;

        template <typename T, typename NativeT = std::decay_t<T>>
        [[nodiscard]] constexpr decltype(auto) get() const noexcept {
            return get_impl<NativeT>();
        }

        template <typename T>
        bool add(T* link) noexcept {
            if (link == nullptr)
                return false;

            bool added = false;
            for_each(links, [&](auto&& field) {
                if (added) return;
                added = field.add(link);
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
                removed = field.remove(link);
            });
            return removed;
        }

        link_holder_tuple(const link_holder_tuple&) = delete;
        link_holder_tuple(link_holder_tuple&&) = delete;
        link_holder_tuple& operator=(const link_holder_tuple&) = delete;
        link_holder_tuple& operator=(link_holder_tuple&&) = delete;
    private:

        template <typename T>
        [[nodiscard]] constexpr decltype(auto) get_impl() const noexcept {
            static_assert(contains<T>(types));
            return links.template get<InnerHolderPolicy<T>>().get();
        }

        constexpr static type_list<Links...> types{ };

        links_t links{ };
    };
}