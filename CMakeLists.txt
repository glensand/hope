cmake_minimum_required(VERSION 3.11)

project(template-guides)
#set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(test)
add_subdirectory(sample)
add_subdirectory(source)