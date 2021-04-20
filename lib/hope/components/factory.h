/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <string_view>
#include <stdexcept>
#include <utility>

namespace hope {

    template <typename ReturnType,
               typename NameClass = std::string_view>
    class factory final {
        using creator_map = std::unordered_map<NameClass, std::function<ReturnType*()>>;
    public:
        factory() = default;
        factory(const factory&) = delete;
        factory& operator=(const factory&) = delete;
        ~factory() = default;

        template <typename Type>
        bool register_object(NameClass name) {
            if (creators.count(name) != 0)
                throw std::runtime_error(std::string("An attempt was made to register already registered type with name: ") + typeid(Type).name());
            creators.emplace(std::move(name), [] { return new Type; });
            return true;
        }

        [[nodiscard]] ReturnType* create(const NameClass& name) const {
            const auto creatorIt = creators.find(name);
            if (creatorIt == std::cend(creators))
                throw std::runtime_error(std::string("Unregistered typename passed to factory of type: ")
                    + typeid(ReturnType).name());
            return creatorIt->second();
        }

    private:
        creator_map creators;
    };
}