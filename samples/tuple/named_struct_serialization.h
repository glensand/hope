/* Copyright (C) 2021 - 2023 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/components/loophole.h"
#include "hope/tuple/tuple_from_struct_unsafe.h"

#include <string_view>
#include <iostream>
#include <sstream>
#include <vector>

namespace hope::sample::named_struct_serializer {

    enum class argument_type {  // for instance network receiver is needed in enumeration of the arg's
        Int,
        Float,
        Bool,
    };

    class named_argument { 
    public:
        virtual ~named_argument() = default;

        virtual void serialize(std::ostream& out) const = 0;

        virtual void deserealize(std::istream& in) = 0;
    };

    template <typename T>
    class named_argument_base : public named_argument {
    public:
        virtual ~named_argument_base() = default;
    
        virtual void serialize(std::ostream& out) const override {
            out << m_name << " "<<  int(m_type) << " " << m_value << std::endl;
        }
    
        virtual void deserealize(std::istream& in) override {
            int type;
            in >> m_name >> type >> m_value; // if stream operation is allowed
            m_type = argument_type(type);
        }

    protected:
        explicit named_argument_base(argument_type type, std::string_view name, const T& value)
            : m_value(value)
            , m_type(type)
            , m_name(name.data()){

        }

        T m_value;

    private:
        argument_type m_type;
        std::string m_name;
    };

    class named_argument_int final : public named_argument_base<int32_t> {
    public:
        explicit named_argument_int(std::string_view name = "", int32_t v = 0)
            : named_argument_base(argument_type::Int, name, v) {

        }
    };

    class named_argument_float final : public named_argument_base<float> {
    public:
        explicit named_argument_float(std::string_view name = "", float v = 0.f)
            : named_argument_base(argument_type::Float, name, v) {

        }
    };

    class named_argument_bool final : public named_argument_base<bool> {
    public:
        explicit named_argument_bool(std::string_view name = "", bool b = false)
            : named_argument_base(argument_type::Bool, name, b) {

        }
    };

    inline
    void arg_map_initializer() {
        loophole::inject<1>(int32_t{}, named_argument_int{});
        loophole::inject<1>(float{}, named_argument_float{});
        loophole::inject<1>(bool{}, named_argument_bool{});
    }

    class query final {
    public:
        explicit query(std::vector<named_argument*>&& args)
            : m_args(std::move(args)) {

        }

        ~query() {
            for (auto* arg : m_args)
                delete arg;
        }

        void serialize(std::ostream& out) {
            for (auto* arg : m_args)
                arg->serialize(out);
        }
    private:
        std::vector<named_argument*> m_args;
    };

    template <typename T>
    struct field final {
        const char* const name{ };
        T value{ };
    };

    struct protocol_version final {
        field<int> major{ "Major" };
        field<int> minor{ "Minor" };
    };

    template <typename T>
    query make_query(const T& st) {
        using tuple_type = decltype(hope::make_tuple<T>());
        auto&& tuple = reinterpret_cast<const tuple_type&>(st);
        std::vector<named_argument*> arguments;

        for_each(tuple, [&] (auto&& field) {
            using clean_t = std::decay_t<decltype(field.value)>;
            using argument_t = decltype(loophole::get<1>(clean_t{}));
            arguments.emplace_back(
                new argument_t(field.name, field.value));
            }
        );

        return query(std::move(arguments));
    }

    inline
    void run() {

        arg_map_initializer();

        protocol_version version/*{ .major.value = 1, .minor.value = 0 }*/; // since cxx20 u may use aggregate init
        version.major.value = 99;
        version.minor.value = 33;

        auto&& q = make_query(version);
        std::stringstream out;
        q.serialize(out);
        std::cout << out.str();
    }
}
