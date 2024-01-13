/* Copyright (C) 2021 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "hope/serialization/struct_serialization.h"

struct custom_struct final {
    int field1;
    bool field2;
};

struct test_struct final {
    int field1;
    custom_struct field2;
};

DECLARE_SERIALIZER(custom_struct) {
    const bool changed = value.field2 != prev_value.field2;
    if (changed)
        pack.write(value.field2);
    return changed;
}

DECLARE_DESERIALIZER(custom_struct) {
    value.field2 = pack.read<bool>();
}

using holder_t = hope::serialization::custom_serializer_holder <
    hope::serialization::entity<custom_struct, custom_struct_serializer, custom_struct_deserializer>
>;

static_assert(holder_t::has<custom_struct>());