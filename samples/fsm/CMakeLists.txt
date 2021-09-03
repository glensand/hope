cmake_minimum_required(VERSION 3.11)

project(fsm_sample)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SAMPLE_HEADERS
    *.h)

file(GLOB SAMPLE_SOURSES
    *.cpp)

add_executable(fsm_sample ${SAMPLE_HEADERS} ${SAMPLE_SOURSES})
target_include_directories(fsm_sample PUBLIC ../../lib)
target_link_libraries(fsm_sample hope)