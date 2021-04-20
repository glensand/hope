cmake_minimum_required(VERSION 3.11)

project(struct_bitfield_inverter)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SAMPLE_HEADERS
    *.h)
	
file(GLOB SAMPLE_SOURSES
    *.cpp)

add_executable(struct_bitfield_inverter ${SAMPLE_HEADERS} ${SAMPLE_SOURSES})
target_include_directories(struct_bitfield_inverter PUBLIC ../../lib)
target_link_libraries(struct_bitfield_inverter hope)