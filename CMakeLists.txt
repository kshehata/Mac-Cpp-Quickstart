cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project("C++ Quickstart")

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(utils utils.cc)

add_executable(example main.cc)
target_link_libraries(example utils)
