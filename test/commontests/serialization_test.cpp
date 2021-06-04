/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
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
        test_set_instance.instance_to_serialize.field_3.field_1 = 116451111.f;
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