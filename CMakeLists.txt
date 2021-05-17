cmake_minimum_required(VERSION 3.16)
project(libant)

set(CMAKE_CXX_STANDARD 17)
set(BIN_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin)

option(BUILD_TEST_CASES "building test cases" OFF)

add_subdirectory(3rdparties)

file(GLOB_RECURSE LIBANT_SOURCE_FILES src/*.cpp)

add_library(ant ${LIBANT_SOURCE_FILES})

if (BUILD_TEST_CASES)
    add_subdirectory(test_cases)
endif ()

target_include_directories(ant PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/3rdparties/fmt/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

set_target_properties(ant
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${BIN_OUTPUT_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${BIN_OUTPUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_DIR}"
        )