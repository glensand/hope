/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <type_traits>

namespace hope::detail {
    template <typename R, typename... Args>
    class function_bridge {
    public:
        virtual ~function_bridge(){}

        [[nodiscard]] virtual function_bridge* clone() const = 0;
        
        virtual R invoke(Args... args) const = 0;
    };

    template <typename Functor, typename R, typename... Args>
    class function_bridge_impl final : public function_bridge<R, Args...> {
    public:
        template <typename ForwardedFunctor>
        explicit function_bridge_impl(ForwardedFunctor&& functor)
            :m_functor(std::forward<ForwardedFunctor>(functor)) {
        }

        function_bridge_impl* clone() const override {
            return new function_bridge_impl(m_functor);
        }

        R invoke(Args... args) const override {
            return m_functor(std::forward<Args>(args)...);
        }

    private:
        Functor    m_functor;
    };
}
