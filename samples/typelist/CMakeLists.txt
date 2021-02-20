cmake_minimum_required(VERSION 3.11)

project(typelist_sample)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SAMPLE_HEADERS
    *.h)
	
file(GLOB SAMPLE_SOURSES
    *.cpp)

add_executable(typelist_sample ${SAMPLE_HEADERS} ${SAMPLE_SOURSES})
target_include_directories(typelist_sample PUBLIC ../../source ../../test)
target_link_libraries(typelist_sample hope)