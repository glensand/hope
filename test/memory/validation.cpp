/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"
#include "sm_objects.h"
#include "registered_types.h"

namespace hope::memory::testing {

    namespace {
        std::vector<std::string_view> string_list{
            "short string",
            "one more string",
            "a little bit longer string",
            "much more longer complicated string",
            "sd longer string as i can imagine, u shouldnt read it",
            "sd longer string as i can imagine, u shouldnt read it anymore",
            "sd longer string as i can imagine, u shouldnt read it anymore. so, i had enough"
        };
    }

    void apply_and_validate_string(const sm_list_modifier_t& modifier, const sm_list_validator_t& validator) {
        sm_list_t sm_list;
        for (const auto& str : string_list)
            sm_list.push_back(new srting_sm_object(str.data()));
        modifier(sm_list);
        ASSERT_TRUE(validator(sm_list));
        for (const auto sm_ptr : sm_list)
            delete sm_ptr;
    }

    TEST(validation, on_string_test)
    {
        apply_and_validate_string(
            [](auto&&) {},
            [](auto&& sm_list) {
                return std::equal(std::begin(sm_list), std::end(sm_list),
                    std::begin(string_list), std::end(string_list),
                    [](auto&& sm_ptr, auto&& str) {
                        return static_cast<const srting_sm_object*>(sm_ptr)->val == str;
                    });
                });
    }

    TEST(validation, on_string_delete_test)
    {
        apply_and_validate_string(
            [](auto&& sm_list) {
                delete sm_list.front();
                sm_list.erase(std::begin(sm_list));
                delete sm_list.back();
                sm_list.erase(std::end(sm_list));
        },
            [](auto&& sm_list)  {
                return std::equal(std::begin(sm_list), std::end(sm_list),
                    std::begin(string_list) + 1, std::end(string_list) - 1,
                    [](auto&& sm_ptr, auto&& str) {
                        return static_cast<const srting_sm_object*>(sm_ptr)->val == str;
                    });
        });
    }

    constexpr auto OverflowCount{ 300 };
    TEST(validation, on_string_chunk_overflow_test)
    {
        constexpr auto list = RegisteredTypesShuffledAlignedTo1{};
        apply_and_validate_string(
            [=](auto&& sm_list) {
                constexpr auto sequence = std::make_index_sequence<size(list)>();
                for (std::size_t i{ 0 }; i < OverflowCount; ++i)
                    fill_vector<simple_sm_object>(sequence, list, sm_list);
            },
            [](auto&& sm_list) {
                return std::equal(std::begin(sm_list), std::end(sm_list),
                    std::begin(string_list) + 1, std::end(string_list),
                    [](auto&& sm_ptr, auto&& str) {
                        return static_cast<const srting_sm_object*>(sm_ptr)->val == str;
                    });
            });
    }
}
