cmake_minimum_required(VERSION 3.11)

project(hope)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB HOPE_HEADERS
    hope/*.h
    hope/fsm/*.h
    hope/core/*.h
    hope/function/*.h
    hope/tuple/*.h
    hope/type_list/*.h
    hope/variant/*.h*
    hope/components/*.h*
    hope/components/singleton_holder/*.h*
    hope/components/policy_map/*.h*
    hope/memory/*.h*
    hope/memory/small_object/*.h*
    hope/concurrency/*.h*
    hope/serialization/*.h*
)

file(GLOB HOPE_SOURSES
    hope/memory/small_object/*.cpp
    hope/serialization/*.cpp
)

add_library(hope STATIC ${HOPE_HEADERS} ${HOPE_SOURSES})
target_include_directories(hope PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})