/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <string_view>

namespace hope::sample {

    class base {
    public:
        virtual ~base() = default;

        virtual std::string_view name() const noexcept = 0;
    };

    class derived1 final : public base {
    public:

        std::string_view name() const noexcept override { return "derived1"; }
    };

    class derived2 final : public base {
    public:

        std::string_view name() const noexcept override { return "derived2"; }
    };

    class derived3 final : public base {
    public:

        std::string_view name() const noexcept override { return "derived3"; }
    };

    class derived4 final : public base {
    public:

        std::string_view name() const noexcept override { return "derived4"; }
    };

    class derived5 final : public base {
    public:

        std::string_view name() const noexcept override { return "derived5"; }
    };

    class derived6 final : public base {
    public:

        std::string_view name() const noexcept override { return "derived6"; }
    };

    class derived7 final : public base {
    public:

        std::string_view name() const noexcept override { return "derived7"; }
    };
}