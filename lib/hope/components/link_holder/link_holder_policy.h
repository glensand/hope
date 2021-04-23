/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <vector>
#include <algorithm>

namespace hope::link_holder_policy {

    template <typename T>
    struct single_value final
    {
        using pointer_t = T*;

        [[nodiscard]] constexpr pointer_t get() const noexcept {
            return pointer;
        }

        template <typename Link>
        [[nodiscard]] bool add(Link* raw_link) {
            if (pointer != nullptr)
                return false;
            if(auto* link = dynamic_cast<T*>(raw_link))
                pointer = link;
            return pointer != nullptr;
        }

        template <typename Link>
        [[nodiscard]] bool remove(Link* raw_link) {
            if (pointer == nullptr)
                return false;
            if(auto* link = dynamic_cast<T*>(raw_link)) {
                if (link == pointer)
                    pointer = nullptr;
            }
            return pointer == nullptr;
        }
    private:
        pointer_t pointer{ nullptr };
    };

    template<typename T>
    struct multiple_value final {

        using value_t = std::vector<T*>;

        [[nodiscard]] constexpr const value_t& get() const noexcept {
            return value;
        }

        template <typename Link>
        [[nodiscard]] bool add(Link* raw_link) {
            auto* link = dynamic_cast<T*>(raw_link);
            return link != nullptr ? add_impl(link) : false;
        }

        template <typename Link>
        [[nodiscard]] bool remove(Link* raw_link) {
            if (value.empty())
                return false;
            auto* link = dynamic_cast<T*>(raw_link);
            return link!= nullptr ? remove_impl(link) : false;
        }

    private:

        bool add_impl(T* link) {
            auto&& it = std::find(std::cbegin(value), std::cend(value), link);
            const bool found = it != std::cend(value);
            if (!found)
                value.push_back(link);
            return !found;
        }

        bool remove_impl(T* link) {
            auto&& it = std::find(std::cbegin(value), std::cend(value), link);
            const bool found = it != std::cend(value);
            if (found)
                value.erase(it);
            return found;
        }

        value_t value;
    };
}