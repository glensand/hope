cmake_minimum_required(VERSION 3.11)

project(hope-main)
#set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(test)
add_subdirectory(samples/memory)
add_subdirectory(samples/typelist)
add_subdirectory(samples/tuple)
add_subdirectory(samples/static_string)
add_subdirectory(source)