/* Copyright (C) 2021 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "logger_test.h"

enum class stub_enum : unsigned {
    element
};

bool enum_written = false;
template<typename TEnum>
void write_enum(hope::log_helper& helper, TEnum object) {
    enum_written = true;
}

struct custom_type final {
    // empty
};

bool custom_type_written = false;
void write(hope::log_helper& helper, const custom_type& object) {
    custom_type_written = true;
}

namespace hope::log::testing {

    TEST_F(logger_test, WriteEnum){
        enum_written = false;
        LOG(log_priority::info) << stub_enum::element;
        ASSERT_TRUE(enum_written);

        enum_written = false;
        LOG(log_priority::error) << stub_enum::element;
        ASSERT_TRUE(enum_written);
    }

    TEST_F(logger_test, WriteCustomType){
        const custom_type object;

        custom_type_written = false;
        LOG(log_priority::info) << object;
        ASSERT_TRUE(custom_type_written);

        custom_type_written = false;
        LOG(log_priority::error) << object;
        ASSERT_TRUE(custom_type_written);
    }

}
