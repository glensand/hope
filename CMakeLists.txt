cmake_minimum_required(VERSION 3.1)

project(template-guides)
#set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set ( CMAKE_CXX_FLAGS "-Werror")
include_directories(src/ src/guides/)

file(GLOB HEADERS
    source/core/*.h
    source/function/*.h
    source/tuple/*.h
    source/type_list/*.h
    source/variant/*.h*
    source/component/*.h*
    source/memory/*.h*
)

add_executable(run source/main.cpp)
add_subdirectory(test)