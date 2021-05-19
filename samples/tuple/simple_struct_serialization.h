/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/components/loophole.h"
#include "hope/tuple/tuple_from_struct_unsafe.h"

#include <iostream>
#include <sstream>
#include <vector>

namespace hope::sample::simple_struct_serializer {

    enum class argument_type {  // for instance network receiver is needed in enumeration of the arg's
        Int,
        Float,
        Bool,
        String,
    };

    class argument { 
    public:
        virtual ~argument() = default;

        virtual void serialize(std::ostream& out) const = 0;

        virtual void deserealize(std::istream& in) = 0;
    };

    template <typename T>
    class argument_base : public argument {
    public:
        virtual ~argument_base() = default;
    
        virtual void serialize(std::ostream& out) const override {
            out << int(m_type) << " " << m_value << std::endl;
        }
    
        virtual void deserealize(std::istream& in) override {
            int type;
            in >> type >> m_value; // if stream operation is allowed
            m_type = argument_type(type);
        }

    protected:
        explicit argument_base(argument_type type, const T& value)
            : m_value(value)
            , m_type(type){

        }

        T m_value;

    private:
        argument_type m_type;
    };

    class argument_int final : public argument_base<int32_t> {
    public:
        explicit argument_int(int32_t v = 0)
            : argument_base(argument_type::Int, v) {

        }
    };

    class argument_float final : public argument_base<float> {
    public:
        explicit argument_float(float v = 0.f)
            : argument_base(argument_type::Float, v) {

        }
    };

    class argument_bool final : public argument_base<bool> {
    public:
        explicit argument_bool(bool b = false)
            : argument_base(argument_type::Bool, b) {

        }
    };

    inline
    void arg_map_initializer() {
        loophole::inject<0>(int32_t{}, argument_int{});
        loophole::inject<0>(float{}, argument_float{});
        loophole::inject<0>(bool{}, argument_bool{});
    }

    class query final {
    public:
        explicit query(std::vector<argument*>&& args) 
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
        std::vector<argument*> m_args;
    };

    struct simple_struct_1 {
        int value1;
        int value2;
        bool value3;
        float value4;
    };

    template <typename T>
    query make_query(const T& st) {
        auto tuple = hope::tuple_from_struct_unsafe(st);

        std::vector<argument*> arguments;

        for_each(tuple, [&] (auto&& field) {
            using clean_t = std::decay_t<decltype(field)>;
            using argument_t = decltype(loophole::get<0>(clean_t{}));
            arguments.emplace_back(new argument_t(std::forward<decltype(field)>(field)));
            });

        return query(std::move(arguments));
    }

    inline
    void run() {

        arg_map_initializer();

        auto st1 = simple_struct_1{ 1, 0, false };
        auto&& query1 = make_query(st1);
        std::stringstream out;
        query1.serialize(out);
        std::cout << out.str();
    }
}
