# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/googletest-src/googletest/include" ../src/guides)

file(GLOB SOURCES
    commontests/*.cpp
)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(tests Tests.cpp)
add_executable(tuple_test commontests/tupletest.cpp)
target_link_libraries(tests gtest_main)
target_link_libraries(tuple_test gtest_main)
add_test(NAME guides_test COMMAND guides_test)
add_test(NAME tuple_test COMMAND tuple_test)