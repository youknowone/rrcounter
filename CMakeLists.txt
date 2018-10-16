cmake_minimum_required(VERSION 2.8.11)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

#target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
#  "${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")
target_include_directories(gtest_main SYSTEM BEFORE INTERFACE
    "${gtest_SOURCE_DIR}/include")
# end of googletest

add_custom_target(
    lint
    COMMAND clang-format
    -i
    */*.hh */*.cc)

add_custom_target(
    lcov
    COMMAND lcov
    --directory . --capture
    --output-file coverage.info 
    &&
    lcov --remove coverage.info '/usr/*' '*/googletest-*/*' '/Applications/*' --output-file coverage.info
    &&
    lcov --list coverage.info
    )

project(rrcounter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# coverage
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CodeCoverage)
APPEND_COVERAGE_COMPILER_FLAGS()
set(COVERAGE_EXCLUDES '*/googletest-*/*' '/usr/*' '/Applications/*')
#setup_target_for_coverage(${PROJECT_NAME}_coverage tests coverage)


include_directories(./)
if (APPLE)
    include_directories(./Pods/libc++/)
endif()

add_executable(rrcounter.test tests/test_rrcounter.cc)
target_link_libraries(rrcounter.test gtest_main)

include(CTest)

#does the application run
add_test(NAME rrcounter_test_run COMMAND rrcounter.test)
