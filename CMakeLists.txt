cmake_minimum_required(VERSION 3.17)
project(libant)

set(CMAKE_CXX_STANDARD 17)
set(BIN_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin)

option(BUILD_TEST_CASES "building test cases" OFF)

# include search dir
include_directories(
   ${PROJECT_SOURCE_DIR}/../
)

#add_library(libant library.cpp library.h)

if (BUILD_TEST_CASES)
    add_subdirectory(test_cases)
endif()