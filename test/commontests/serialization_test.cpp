/* Copyright (C) 2021 - 2023 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"
#include "hope/serialization/struct_serialization.h"
#include "hope/tuple/tuple_utils.h"

namespace
{

    struct simple_struct final {
        int field_1{ 0 };
        float field_2{ 0.0f };
        bool field_3{ false };
    };

    struct recursive_struct final {
        int field_1{ 0 };
        float field_2{ 0.0f };
        simple_struct field_3{  };
    };

    struct deep_recursive_struct final {
        float field_1{ 1.f };
        float field_2{ 0.0f };
        simple_struct field_3;
        recursive_struct field_4;
    };

    template<typename TStruct>
    struct test_set final {
        TStruct instance_to_serialize;
        TStruct instance_to_deserialize;
        hope::serialization::pod_serializer<TStruct> serializer{ instance_to_serialize };
        hope::serialization::pod_serializer<TStruct> deserializer{ instance_to_deserialize };
        hope::serialization::package pack;
    };

    template<typename TStruct>
    void run_simple_test() {
        test_set<TStruct> test_set_instance;

        test_set_instance.serializer.serialize(test_set_instance.pack);
        test_set_instance.deserializer.deserialize(test_set_instance.pack);

        ASSERT_TRUE(test_set_instance.instance_to_deserialize == test_set_instance.instance_to_serialize);

        test_set_instance.pack.clear();
        test_set_instance.instance_to_serialize.field_1 = 11;
        test_set_instance.instance_to_serialize.field_2 = 0.42f;
        test_set_instance.serializer.serialize(test_set_instance.pack);
        test_set_instance.deserializer.deserialize(test_set_instance.pack);

        ASSERT_TRUE(test_set_instance.instance_to_deserialize == test_set_instance.instance_to_serialize);
    }

    template<typename TStruct>
    void run_deep_test() {
        test_set<TStruct> test_set_instance;
        
        test_set_instance.instance_to_serialize.field_1 = 11;
        test_set_instance.instance_to_serialize.field_2 = 111111.f;
        test_set_instance.instance_to_serialize.field_3.field_1 = 116451111;
        test_set_instance.instance_to_serialize.field_3.field_3 = true;
        test_set_instance.serializer.serialize(test_set_instance.pack);
        test_set_instance.deserializer.deserialize(test_set_instance.pack);

        ASSERT_TRUE(test_set_instance.instance_to_deserialize == test_set_instance.instance_to_serialize);
    }
}

TEST(SerializationTest, SimpleStructSerialization)
{
    run_simple_test<simple_struct>();
}

TEST(SerializationTest, RecursiveStructSerialization)
{
    run_simple_test<recursive_struct>();
    run_deep_test<recursive_struct>();
}

TEST(SerializationTest, DeepStructSerialization)
{
    run_simple_test<deep_recursive_struct>();
    run_deep_test<deep_recursive_struct>();

    test_set<deep_recursive_struct> test_set_instance;
}

DECLARE_SERIALIZER(simple_struct) {
    const bool changed = value.field_3 != prev_value.field_3; // serialize only third field
    if (changed)
        pack.write(value.field_3);
    return changed;
}

DECLARE_DESERIALIZER(simple_struct) {
        value.field_3 = pack.read<bool>();
}

TEST(SerializationTest, CustomSerialization)
{
    recursive_struct instance_to_serialize;
    recursive_struct instance_to_deserialize;
    hope::serialization::package pack;

    using serializer_t = hope::serialization::pod_serializer<
        recursive_struct,
        hope::serialization::entity<simple_struct, simple_struct_serializer, simple_struct_deserializer>
    >;

    serializer_t serializer{ instance_to_serialize };
    serializer_t deserializer{ instance_to_deserialize };

    serializer.serialize(pack);
    deserializer.deserialize(pack);
    ASSERT_TRUE(instance_to_deserialize == instance_to_serialize);

    pack.clear();
    instance_to_serialize.field_3.field_3 = true;
    serializer.serialize(pack);
    deserializer.deserialize(pack);

    ASSERT_TRUE(instance_to_deserialize == instance_to_serialize);

    pack.clear();
    instance_to_serialize.field_3.field_1 = 111;
    serializer.serialize(pack);
    deserializer.deserialize(pack);

    ASSERT_TRUE(!(instance_to_deserialize == instance_to_serialize));
}