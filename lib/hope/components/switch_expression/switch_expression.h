/* Copyright (C) 2021 - 2023 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <tuple>
#include "hope/typelist/type_list.h"

namespace hope {

    template<typename... Ts>
    class switch_expression_member final {
        static_assert(sizeof ...(Ts) > 0);
    public:
        explicit switch_expression_member(const Ts&... args)
            : expression(args...) {

        }

        template<typename TKey, typename TClass, typename... TVs>
        bool apply(TClass& instance, const TKey& key, TVs&&... args) {
            return evaluate(instance, key, std::make_index_sequence<sizeof ...(Ts)>{}, std::forward<TVs>(args)...);
        }

    private:
        template<typename TKey, typename TClass, std::size_t...Is, typename... TVs>
        bool evaluate(TClass& instance, const TKey key, std::index_sequence<Is...>, TVs&&... args) {
            return (... || try_apply<Is>(instance, key, std::forward<TVs>(args)...));
        }

        template<std::size_t I, typename TKey, typename TClass, typename... TVs>
        bool try_apply(TClass& Instance, const TKey key, TVs&&... args) {
            if constexpr (I + 1 < sizeof ...(Ts)) {
                // Even is a key, odd is a function
                if constexpr (I % 2 == 0) {
                    // todo:: check if callable
                    if (key == std::get<I>(expression)) {
                        auto* function = std::get<I + 1>(expression);
                        (Instance.*function)(std::forward<TVs>(Args)...);
                        return true;
                    }
                }
            }
            return false;
        }

        std::tuple<Ts...> expression;
    };

    template<typename... Ts>
    switch_expression_member(Ts...) -> switch_expression_member<Ts...>;


    template<typename... Ts>
    class switch_expression_lambda final {
        static_assert(sizeof ...(Ts) > 0);

    public:
        explicit switch_expression_lambda(const Ts&... args)
            : expression(args...) {

        }

        template<typename TKey, typename... TVs>
        bool apply(const TKey& key, TVs&&... vals) {
            return evaluate(key, std::make_index_sequence<sizeof ...(Ts)>{}, std::forward<TVs>(vals)...);
        }

    private:
        template<typename TKey, std::size_t...Is, typename... TVs>
        bool evaluate(const TKey& key, std::index_sequence<Is...>, TVs&&... vals) {
            return (... || try_apply<Is>(key, std::forward<TVs>(vals)...));
        }

        template<std::size_t I, typename TKey, typename... TVs>
        bool try_apply(const TKey& key, TVs&&... vals) {
            if constexpr (I + 1 < sizeof ...(Ts)) {
                // Even is a key, odd is a function
                if constexpr (I % 2 == 0) {
                    // todo:: check if callable
                    if (key == std::get<I>(expression)) {
                        auto&& function = std::get<I + 1>(expression);
                        function(std::forward<TVs>(vals)...);
                        return true;
                    }
                }
            }
            return false;
        }

        std::tuple<Ts...> expression;
    };

    template<typename... Ts>
    switch_expression_lambda(Ts...) -> switch_expression_lambda<Ts...>;

}