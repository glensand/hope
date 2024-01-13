/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "hope/components/detector.h"

namespace {

    struct custom_struct final { };
    void write(custom_struct& st){ }

    template<typename T>
    using write_function_t = decltype(write(std::declval<T&>()));

    static_assert(hope::is_detected_v<write_function_t, custom_struct>, "");

}
