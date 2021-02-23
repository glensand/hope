/* Copyright (C) 2020 - 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "components/loophole.h"
#include "tuple/tuple_from_struct.h"

#include <string_view>
#include <iostream>
#include <sstream>
#include <vector>

namespace hope::sample::struct_serializer {

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
            out << int(m_type) << " " << m_name << " " << m_value << std::endl;
        }
    
        virtual void deserealize(std::istream& in) override {
            int type;
            in >> type >> m_name >> m_value; // if stream operation is allowed
            m_type = argument_type(type);
        }
    
        const T& value() const noexcept  {
            return m_value;
        }

        argument_type type() const noexcept {
            return m_type;
        }

        const std::string& name() const noexcept {
            return m_name;
        }

    protected:
        explicit argument_base(argument_type type, std::string_view name, const T& value)
            : m_value(value)
            , m_name(name.data())
            , m_type(type){

        }

        T m_value;

    private:
        std::string m_name;
        argument_type m_type;
    };

    class argument_int final : public argument_base<int32_t> {
    public:
        explicit argument_int(std::string_view name = "", int32_t v = 0)
            : argument_base(argument_type::Int, name, v) {

        }
    };

    class argument_float final : public argument_base<float> {
    public:
        explicit argument_float(std::string_view name = "", float v = 0.f)
            : argument_base(argument_type::Float, name, v) {

        }
    };

    class argument_bool final : public argument_base<bool> {
    public:
        explicit argument_bool(std::string_view name = "", bool b = false)
            : argument_base(argument_type::Bool, name, b) {

        }
    };

    class argument_string final : public argument_base<std::string> {
    public:
        explicit argument_string(std::string_view name = "", std::string s = "")
            : argument_base(argument_type::String, name, s) {

        }
    };

    void arg_map_initializer() {
        loophole::inject(int32_t{}, argument_int{});
        loophole::inject(float{}, argument_float{});
        loophole::inject(bool{}, argument_bool{});
        loophole::inject(std::string{}, argument_string{});
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
        auto tuple = hope::tuple_from_struct_unsafe<T>(st);

        std::vector<argument*> arguments;

        for_each(tuple, [&] (auto&& field) {
            using clean_t = std::decay_t<decltype(field)>;
            using argument_t = decltype(loophole::get(clean_t{}));
            arguments.emplace_back(new argument_t("Argument name", std::forward<decltype(field)>(field)));
            });

        return query(std::move(arguments));
    }
    
    void run() {

        arg_map_initializer();

        auto st1 = simple_struct_1{ 1, 0, "great value" };
        auto&& query1 = make_query(st1);
        std::stringstream out;
        query1.serialize(out);
        std::cout << out.str();
    }
}