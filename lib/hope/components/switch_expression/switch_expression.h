/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <any>
#include <cassert>
#include <unordered_map>
#include <functional>

namespace hope {

    template<typename Key>
    class switch_expression final {
        using functor_t = std::function<void()>;
        using switch_t = std::unordered_map<Key, functor_t>;
        using this_t = switch_expression<Key>;

        class key_value_pair final {
        public:
            key_value_pair(this_t& master, const Key& key)
                : m_key(key)
                , m_master(master) {}

            template<typename Functor>
            this_t& operator()(Functor&& function) {
                m_master.m_switch[m_key] = std::forward<Functor>(function);
                return m_master;
            }

        private:
            const Key& m_key;
            this_t& m_master;
        };

        friend class key_value_pair;
    public:
        switch_expression(const Key& key)
            : m_key(key) { }

        ~switch_expression() {
            if (auto && it = m_switch.find(m_key); it != std::end(m_switch))
                it->second();
        }

        key_value_pair operator[](const Key& key) {
            const auto it = m_switch.find(key);
            assert(it == std::cend(m_switch));
            (void)it; // remove clang unused variable warning
            auto&& [elementIt, _] = m_switch.emplace(key, functor_t{});
            return { *this, elementIt->first };
        }
     
    private:
        Key m_key;
        switch_t m_switch;
    };

    template<typename Key>
    switch_expression(Key)->switch_expression<std::decay_t<Key>>;
}