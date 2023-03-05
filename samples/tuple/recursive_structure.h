/* Copyright (C) 2021 - 2023 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

namespace hope::sample {

    struct recursive_structure final {

        int field0;
        float field2;

        struct inner_structure1 final {
            int field1;
            int field2;

            struct inner_structure2 final {
                int field1;
                bool field2;
            };

            inner_structure2 field3;
        };
        inner_structure1 field3;
    };
}
