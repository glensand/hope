#pragma once

#include "memory/small_object.h"

template <typename T>
struct sm_object_5 : hope::memory::small_object{
    T _1, _2, _3, _4, _5;
};

template <typename T>
struct sm_object_4 : hope::memory::small_object {
    T _1, _2, _3, _4;
};

template <typename T>
struct sm_object_3 : hope::memory::small_object {
    T _1, _2, _3;
};

template <typename T>
struct sm_object_2 : hope::memory::small_object {
    T _1, _2;
};

template <typename T>
struct sm_object_1 : hope::memory::small_object {
    T _1;
};

template <typename T>
struct sm_object_0 : hope::memory::small_object {

};

using bytes_1 = std::aligned_storage_t<1, 1>;
using bytes_2 = std::aligned_storage_t<2, 1>;
using bytes_3 = std::aligned_storage_t<3, 1>;
using bytes_4 = std::aligned_storage_t<4, 1>;
using bytes_5 = std::aligned_storage_t<5, 1>;
using bytes_6 = std::aligned_storage_t<6, 1>;
using bytes_7 = std::aligned_storage_t<7, 1>;
using bytes_8 = std::aligned_storage_t<8, 1>;
using bytes_9 = std::aligned_storage_t<9, 1>;
using bytes_10 = std::aligned_storage_t<10, 1>;
using bytes_11 = std::aligned_storage_t<11, 1>;