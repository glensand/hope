cmake_minimum_required(VERSION 3.11)

project(hope)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB HOPE_HEADERS
    core/*.h
    function/*.h
    tuple/*.h
    type_list/*.h
    variant/*.h*
    components/*.h*
    components/singleton_holder/*.h*
    memory/*.h*
    memory/small_object/*.h*
    concurrency/*.h*
)

file(GLOB HOPE_SOURSES
    memory/small_object/*.cpp
)
add_library(hope STATIC ${HOPE_HEADERS} ${HOPE_SOURSES})

