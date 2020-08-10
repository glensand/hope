cmake_minimum_required(VERSION 3.11)

project(sample)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SAMPLE_HEADERS
    inc/*.h)
	
file(GLOB SAMPLE_SOURSES
    src/*.cpp)

add_executable(sample ${SAMPLE_HEADERS} ${SAMPLE_SOURSES})
target_include_directories(sample PUBLIC ../source)
target_link_libraries(sample hope)