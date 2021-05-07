/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include <cassert>

#include "hope/serialization/struct_serialization.h"
#include "hope/tuple/tuple_utils.h"

namespace {
    struct sample_struct final {
        int field_1{ 0 };
        float field_2{ 0.0f };
        bool field_3{ false };
    };    
}

int main()
{
    sample_struct instance_to_serialize;
    sample_struct instance_to_deserialize;
    hope::serialization::pod_serializer serializer(instance_to_serialize);
    hope::serialization::pod_serializer deserializer(instance_to_deserialize);
    hope::serialization::package pack;

    serializer.serialize(pack);
    deserializer.deserialize(pack);

    assert(instance_to_deserialize == instance_to_serialize);

    instance_to_serialize.field_1 = 11;
    instance_to_serialize.field_2 = 0.42f;

    serializer.serialize(pack);
    deserializer.deserialize(pack);

    assert(instance_to_deserialize == instance_to_serialize);

	return 0;
} 