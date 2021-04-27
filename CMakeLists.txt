cmake_minimum_required(VERSION 3.11)

project(hope-main)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(test)
add_subdirectory(samples/memory)
add_subdirectory(samples/typelist)
add_subdirectory(samples/tuple)
add_subdirectory(samples/typemap)
add_subdirectory(samples/static_string)
add_subdirectory(samples/singleton_holder)
add_subdirectory(samples/struct_bitfield_inverter)
add_subdirectory(samples/fsm)
add_subdirectory(lib)