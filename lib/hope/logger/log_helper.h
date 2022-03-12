/* Copyright (C) 2021 - 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/logger/stack_buffer.h"
#include "hope/logger/log_level.h"

#include "hope/components/detector.h"

#include <charconv>

namespace hope {

    class logger;

    /**
     * Helper class for log writing, provide dispatch of the writing value according their type
     */
    struct log_helper final {

        log_helper(logger& logger_instance, log_priority priority);
        ~log_helper();

        /**
         * Pseudonym of the method which might be used to write log in specific form
         * To use specific rules, create method of desired class with this signature:
         * void write(log_helper&)
         */
        template <typename T>
        using write_method_t = decltype(std::declval<T>().write(std::declval<log_helper&>()));

        /**
         * Pseudonym to the global write function. Template and even are suitable.
         * To use custom write rule to the desired type, create function with next signature:
         * template<typename T>
         * void write(log_helper&, const T&);
         * Or explicitly declare type of the variable
         */
        template<typename T>
        using write_function_t = decltype(write(std::declval<log_helper&>(), std::declval<const T&>()));

        /**
         * Pseudonym to the enum writing function. To use special rule for enums declare:
         * template<typename TEnum>
         * void write_enum(log_helper&, TEnum enum_instance);
         */
        template<typename T>
        using write_enum_function_t = decltype(write_enum(std::declval<log_helper&>(), std::declval<const T&>()));

        /**
         * Thin wrapper around object of specified class; Is used to write value in this fprm
         * [VALUE]
         * @tparam T Type of the containing object
         */
	    template <typename T>
	    struct value_wrapper final {
            value_wrapper(const T& val)
			    : value(val) { }
		
		    const T& value;
	    };

        /**
        * Central point of writing operation, provide type dispatch and call desired write method.
        * If the type is enum and write_enum had been detected, thus it will be called
        * If member function with signature void write(log_helper&) detected, it will be called
        * If global function with signature void write(log_helper, const T&) detected, it will be called
        * @tparam T Type of the object to be written
        * @param helper Self
        * @param value Instance of the object to be written
        * @return Self
        */
        template<typename T>
        friend log_helper& operator<<(log_helper& helper, const T& value){
            if constexpr (is_detected_v<write_method_t, T>) {
                value.write(helper);
            } else if constexpr (is_detected_v<write_function_t, T>) {
                write(helper, value);
            } else if constexpr ( std::is_enum_v<T>){
                if constexpr(is_detected_v<write_enum_function_t, T>)
                    write_enum(helper, value);
                else
                    helper.write_impl(int(value)); // if enum writer had not been set, write enum as integer
            } else {
                helper.write_impl(value);
            }
            return helper;
        }

        /**
         *  Function needed to delegate call to the same operator<<, but with a little different semantic
         *  On MSVC it does not need, but another compilers can not call function with non const
         *  reference parameter with rvalue
         */
        template<typename T>
        friend log_helper& operator<<(log_helper&& helper, const T& value) {
            helper << value;
            return helper;
        }

        /**
         * Wrap value to store it with pretty form: [VALUE]
         */
	    template <typename T>
	    static value_wrapper<T> build_value(const T& value) {
		    return value_wrapper<T>(value);
	    }
    private:

        void write_impl(const void* data, std::size_t size);
        void write_impl(std::string_view data);
        void write_impl(const std::string& data);

        template<typename T>
        void write_impl(const value_wrapper<T>& object){
            m_buffer.put(" [", 2);
            write(object);
            m_buffer.put("] ", 2);
        }

        template<typename T>
        void write_impl(const T& val) {
            constexpr bool is_suitable = std::is_integral_v<T> || std::is_floating_point_v<T>;
            if constexpr (is_suitable) {
                std::array<char, 100> buffer{};
                auto&& [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), val);
                write_impl(buffer.data(), std::size_t(ptr) - std::size_t(buffer.data()));
            }
            static_assert(is_suitable, "<--- hope::logger: An attempt was made to write value of unregistered type.");
        }

	    stack_buffer m_buffer;	// Contains all written bytes
	    logger&	m_logger;	// Ref to the global logger, all bytes had to be written there
        log_priority m_priority;
    };

}

#define HOPE_INTERIOR_LOG(PRIORITY, logger) if((logger).should_write(PRIORITY)) hope::log_helper(logger, PRIORITY) <<
#define HOPE_INTERIOR_LOG_TRACE(PRIORITY, logger) if((logger).should_write(PRIORITY)) hope::log_helper(logger, PRIORITY) <<  __FUNCTION__ << " "
#define HOPE_VAL(V) hope::log_helper::build_value(V)