cmake_minimum_required(VERSION 3.1)

project(template-guides)
#set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set ( CMAKE_CXX_FLAGS "-Werror")
include_directories(src/ src/guides/)

file(GLOB HEADERS
    src/guides/core/*.h
    src/guides/fastpimpl/tree/*.h
    src/guides/function/expression/*.h
    src/guides/tuple/*.h
    src/guides/typeList/*.h
    src/guides/valueBasedWrappers/*.h
    src/guides/variant/*.h*
)

add_executable(run src/guides/main.cpp)
add_subdirectory(test)