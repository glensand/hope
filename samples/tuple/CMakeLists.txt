cmake_minimum_required(VERSION 3.11)

project(tuple_sample)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SAMPLE_HEADERS
    *.h)

file(GLOB SAMPLE_SOURSES
    *.cpp)

add_executable(tuple_sample ${SAMPLE_HEADERS} ${SAMPLE_SOURSES})
target_include_directories(tuple_sample PUBLIC ../../lib)
target_link_libraries(tuple_sample hope)