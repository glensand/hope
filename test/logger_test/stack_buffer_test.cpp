/* Copyright (C) 2021 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "hope/logger/stack_buffer.h"
#include <gtest/gtest.h>

namespace hope::log::testing{

    void test_heap_buffer(std::size_t bytes_count){
        std::vector<char> heap_test_buffer;
        for(std::size_t i{ 0 }; i < bytes_count; ++i)
            heap_test_buffer.push_back(127 - (int)i % 255);

        hope::stack_buffer buffer;
        buffer.put(heap_test_buffer.data(), bytes_count);

        auto&& stack_buffer_impl = buffer.buffer;
        auto&& heap_buffer = buffer.additional_buffer;

        // check all the data related to stack buffer written successfully
        std::size_t data_on_stack_to_check = bytes_count;
        if(bytes_count > stack_buffer_impl.size())
            data_on_stack_to_check = stack_buffer_impl.size();

        for(std::size_t i{ 0 }; i < data_on_stack_to_check; ++i)
            ASSERT_TRUE(stack_buffer_impl[i] == heap_test_buffer[i]);

        if(bytes_count > stack_buffer_impl.size()){
            // check heap related fata
            for(std::size_t i{0}; i < heap_buffer.size(); ++i){
                auto&& from_heap_buffer = heap_buffer[i];
                auto&& from_test_buffer = heap_test_buffer[stack_buffer_impl.size() + i];
                ASSERT_TRUE(from_heap_buffer == from_test_buffer);
            }
        }
    }

    TEST(StackBufferTest, AddToStack) {
        hope::stack_buffer buff;
        constexpr std::string_view test_buffer = "TestBuffer";
        buff.put(test_buffer.data(), test_buffer.size());

        ASSERT_TRUE(test_buffer.size() == buff.bytes_written);
        const std::string_view written_data(buff.buffer.data(), buff.bytes_written);
        ASSERT_TRUE(test_buffer == written_data);
    }

    TEST(StackBufferTest, AddZeroBytes){
        test_heap_buffer(1000);
    }

    TEST(StackBufferTest, Add1Byte){
        test_heap_buffer(1000);
    }

    TEST(StackBufferTest, Add1000Bytes){
        test_heap_buffer(1000);
    }

    TEST(StackBufferTest, AddStackBufferSizeMinus1Bytes) {
        test_heap_buffer(hope::stack_buffer::BufferSize - 1);
    }

    TEST(StackBufferTest, AddStackBufferSizeBytes) {
        test_heap_buffer(hope::stack_buffer::BufferSize);
    }

    TEST(StackBufferTest, AddStackBufferSizePlus1Bytes) {
        test_heap_buffer(hope::stack_buffer::BufferSize + 1);
    }

    TEST(StackBufferTest, Add2000Bytes) {
        test_heap_buffer(2000);
    }

}
