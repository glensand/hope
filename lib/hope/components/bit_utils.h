/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

namespace hope::bit_utils {

    constexpr unsigned bit_count(unsigned value) {
        unsigned result = 0;

        while (value) {
            value &= (value - 1);
            ++result;
        }

        return result;
    }

}
