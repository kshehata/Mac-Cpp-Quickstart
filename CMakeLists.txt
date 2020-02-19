cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project("C++ Quickstart")

# Tell CMake that we need unit tests generated
enable_testing()
# Even though we're not using it here, set default to C++ 2014
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup for Conan dependencies
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Library for our utility functions
add_library(utils utils.cc)

# Unit tests for utility functions
# Note: I prefer having unit tests beside the code to reinforce that unit tests
# *are* a part of the code. Others like having them in a separate directory,
# which has the advantage that you can separate the dependencies. Feel free
# to move the tests to a separate folder, or even the entire utils library, as
# an exercise.
add_executable(UtilsTest utils_test.cc)
target_link_libraries(UtilsTest utils ${CONAN_LIBS})
add_test(UtilsTest bin/UtilsTest)

# Main executable linked with our library and dependencies
add_executable(example main.cc)
target_link_libraries(example utils ${CONAN_LIBS})
