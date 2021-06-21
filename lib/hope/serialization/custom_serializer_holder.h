/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/typelist/type_list.h"

namespace hope::serialization {

    class package;

    namespace detail {

        struct entity_tag {};

    }

    template<typename Tag, typename Serializer, typename Deserializer>
    struct entity final : detail::entity_tag{
        using tag_t = Tag;
        using serializer_t = Serializer;
        using deserializer_t = Deserializer;
    };

    template<typename... Serializer>
    class custom_serializer_holder final {

        constexpr void type_check() {
            hope::for_each(type_list<Serializer...>{}, [](auto type_holder) {
                using clear_t = typename std::decay_t<decltype(type_holder)>::Type;
                static_assert(std::is_base_of_v<detail::entity_tag, clear_t>);
            });
        }

    public:
        template<typename Tag>
        constexpr static bool has() {
            bool has_type{ false };
            hope::for_each(type_list<Serializer...>{}, [&](auto type_holder) {
                using clear_t = typename std::decay_t<decltype(type_holder)>::Type;
                has_type |= std::is_same_v<typename clear_t::tag_t, Tag>;
            });
            return has_type;
        }

        template<typename Tag>
        constexpr static auto serializer() {
            constexpr auto index = index_of<Tag>();
            using entity_t = NthType<index, Serializer...>;
            return type_holder<typename entity_t::serializer_t>();
        }

        template<typename Tag>
        constexpr static auto deserializer() {
            constexpr auto index = index_of<Tag>();
            using entity_t = NthType<index, Serializer...>;
            return type_holder<typename entity_t::deserializer_t>();
        }

    private:
        template <typename Tag>
        constexpr static std::size_t index_of() {
            return find_if(type_list<Serializer...>{ }, [](auto type_holder) {
                using clear_t = typename std::decay_t<decltype(type_holder)>::Type;
                return std::is_same_v<Tag, typename clear_t::tag_t>;
            });
        }
    };

}
