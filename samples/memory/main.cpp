/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include <chrono>
#include <iostream>
#include <fstream>
#include <functional>
#include <iostream>

#include "hope/typelist/type_list.h"
#include "hope/memory/small_object/small_object.h"

#include "memory/sm_objects.h"
#include "memory/objects_aligned_to4.h"

namespace hope::memory::testing {

    using func_to_measure = std::function<void()>;
    using ptr_list_t = std::vector<void*>;

    template <typename T>
    bool alloc_impl(std::size_t size) {
        for (std::size_t i{ 0 }; i < size; ++i)
            new T{ };
        return true;
    }

    constexpr std::size_t MaxChunkCount{ 250 };

    template<template<typename Inner> typename T, typename Empty,
    std::size_t... Is, typename... Ts>
    void alloc_seq_by_all(std::index_sequence<Is...>, type_list<Ts...>list, std::size_t size) {
        // so, my msvc cant use fold expressions properly, thus i have to do such shitty trick with curly braces
        bool br[] = { alloc_impl<T<typename decltype(get_nth_type<Is>(list))::Type>>(size)... };
        (void)br; // br is needed for pack expansion
        alloc_impl<Empty>();
    }

    template<template<typename Inner> typename T, typename Empty,
    std::size_t... Is, typename... Ts>
    void alloc_seq(std::index_sequence<Is...>, type_list<Ts...>list, std::size_t size) {
        for(std::size_t i{ 0 }; i < size; ++i) {
            bool br[] = { bool(new T<typename decltype(get_nth_type<Is>(list))::Type>())... };
            (void)br; // br is needed for pack expansion
            new Empty();
        }
    }

    template<template<typename Inner> typename T, typename Empty,
        std::size_t... Is, typename... Ts>
    void fill_seq(std::index_sequence<Is...>, type_list<Ts...> list, size_t size, ptr_list_t &ptr_list) {
        for (std::size_t i{ 0 }; i < size; ++i) {
            void* br[] = { new T<typename decltype(get_nth_type<Is>(list))::Type>()... };
            ptr_list.insert(std::end(ptr_list), std::begin(br), std::end(br));
            ptr_list.push_back(new Empty());
        }
    }

    auto alloc_and_clock(func_to_measure&& func) {
        const auto start_time = std::chrono::steady_clock::now();
        func();
        const auto end_time = std::chrono::steady_clock::now();
        const auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        return elapsed_ns;
    }

    template <typename... Ts>
    void perform_alloc_test(type_list<Ts...> list, const std::string_view file_name) {
        std::ofstream stream(file_name.data());
        constexpr auto sequence = std::make_index_sequence<size(list)>();
        stream << "Count" << '\t' << "Sm clock" << '\t' << "Std clock" << std::endl;
        for (std::size_t i{ 100 }; i < MaxChunkCount; i += 10) {
            const auto sm_clock = alloc_and_clock([=](){alloc_seq<simple_sm_object, empty_sm>(sequence, list, i); });
            sm_allocator_reset::apply();
            const auto std_clock = alloc_and_clock([=]() {alloc_seq<sm_object_std, empty_std>(sequence, list, i); });
            stream << i << '\t' << sm_clock.count() << '\t' << std_clock.count() << std::endl;
        }
        stream.close();
    }

    template <typename... Ts>
    void perform_alloc_dealloc_test(type_list<Ts...> list, const std::string_view file_name) {
        std::ofstream stream(file_name.data());
        constexpr auto sequence = std::make_index_sequence<size(list)>();
        stream << "Count" << '\t' << "Sm clock" << '\t' << "Std clock" << std::endl;
        for (std::size_t i{ 100 }; i < MaxChunkCount; i += 10) {
            const auto sm_clock = alloc_and_clock([=]() {
                ptr_list_t ptr_list(i * size(list));
                fill_seq<simple_sm_object, empty_sm>(sequence, list, i, ptr_list);
                for (auto ptr : ptr_list)
                    delete static_cast<small_object*>(ptr);
            });
            sm_allocator_reset::apply();
            const auto std_clock = alloc_and_clock([=]() {
                ptr_list_t ptr_list(i * size(list));
                fill_seq<sm_object_std, empty_std>(sequence, list, i, ptr_list);
                for (auto ptr : ptr_list)
                    delete static_cast<empty_std*>(ptr);
            });
            stream << i << '\t' << sm_clock.count() << '\t' << std_clock.count() << std::endl;
        }
        stream.close();
    }

    template <typename... Ts>
    void perform_dealloc_test(type_list<Ts...> list, const std::string_view file_name) {
        std::ofstream stream(file_name.data());
        constexpr auto sequence = std::make_index_sequence<size(list)>();
        stream << "Count" << '\t' << "Sm clock" << '\t' << "Std clock" << std::endl;
        for (std::size_t i{ 100 }; i < MaxChunkCount; i += 10) {
            ptr_list_t ptr_list;
            fill_seq<simple_sm_object, empty_sm>(sequence, list, i, ptr_list);
            const auto sm_clock = alloc_and_clock([&]() {
                for (auto ptr : ptr_list)
                    delete static_cast<small_object*>(ptr);
                });
            ptr_list.clear();
            sm_allocator_reset::apply();
            fill_seq<sm_object_std, empty_std>(sequence, list, i, ptr_list);
            const auto std_clock = alloc_and_clock([=]() {
                for (auto ptr : ptr_list)
                    delete static_cast<empty_std*>(ptr);
                });
            stream << i << '\t' << sm_clock.count() << '\t' << std_clock.count() << std::endl;
        }
        stream.close();
    }
}

template <typename... Ts, std::size_t... Is>
void print_size(hope::type_list<Ts...> list, std::index_sequence<Is...> seq){
    std::size_t br[] = { sizeof(simple_sm_object<typename decltype(hope::get_nth_type<Is>(list))::Type>)...  };
    for (auto&& i : br)
        std::cout << i << " ";
}

int main() {
    hope::memory::testing::perform_alloc_test(RegisteredTypesAscendingAlignedTo4{}, "AllocAscending4.csv");
    hope::memory::testing::perform_alloc_test(RegisteredTypesDescendingAlignedTo4{}, "AllocDescending4.csv");
    hope::memory::testing::perform_alloc_test(RegisteredTypesShuffledAlignedTo4{}, "AllocShuffled4.csv");

    hope::memory::testing::perform_alloc_dealloc_test(RegisteredTypesAscendingAlignedTo4{}, "AllocDeallocAscending4.csv");
    hope::memory::testing::perform_alloc_dealloc_test(RegisteredTypesDescendingAlignedTo4{}, "AllocDeallocDescending4.csv");
    hope::memory::testing::perform_alloc_dealloc_test(RegisteredTypesShuffledAlignedTo4{}, "AllocDeallocShuffled4.csv");

    hope::memory::testing::perform_dealloc_test(RegisteredTypesAscendingAlignedTo4{}, "DeallocAscending4.csv");
    hope::memory::testing::perform_dealloc_test(RegisteredTypesDescendingAlignedTo4{}, "DeallocDescending4.csv");
    hope::memory::testing::perform_dealloc_test(RegisteredTypesShuffledAlignedTo4{}, "DeallocShuffled4.csv");

    print_size(RegisteredTypesShuffledAlignedTo4{}, std::make_index_sequence < size(RegisteredTypesShuffledAlignedTo4{}) >() );

    return 0;
} 