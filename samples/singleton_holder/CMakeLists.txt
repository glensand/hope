cmake_minimum_required(VERSION 3.11)

project(singleton_holder_sample)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SAMPLE_HEADERS
    *.h)
	
file(GLOB SAMPLE_SOURSES
    *.cpp)

add_executable(singleton_holder_sample ${SAMPLE_HEADERS} ${SAMPLE_SOURSES})
target_include_directories(singleton_holder_sample PUBLIC ../../lib ../../test)
target_link_libraries(singleton_holder_sample hope)