cmake_minimum_required(VERSION 3.11)

project(memory_sample)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SAMPLE_HEADERS
    *.h)

file(GLOB SAMPLE_SOURSES
    *.cpp)

add_executable(memory_sample ${SAMPLE_HEADERS} ${SAMPLE_SOURSES})
target_include_directories(memory_sample PUBLIC ../../lib ../../test)
target_link_libraries(memory_sample hope)