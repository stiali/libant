cmake_minimum_required(VERSION 3.10)
project(libant)

set(CMAKE_CXX_STANDARD 17)
set(BIN_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin)

option(BUILD_TEST_CASES "building test cases" OFF)

file(GLOB_RECURSE LIBANT_SOURCE_FILES src/*.cpp)
if (FILE_SYSTEM_NOT_SUPPORTED)
    list(REMOVE_ITEM LIBANT_SOURCE_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/file_utils/file_purger.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/logger/logger.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/logger/fmt/format.cpp)
endif ()

# OpenSSL
find_package(OpenSSL)
if (OpenSSL_FOUND)
    message(STATUS "OpenSSL found")
    include_directories(${OPENSSL_INCLUDE_DIR})
else ()
    message(STATUS "OpenSSL not found")
    list(REMOVE_ITEM LIBANT_SOURCE_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/hash/md5.cpp)
endif ()


# include search dir
include_directories(
        ${PROJECT_SOURCE_DIR}/../
)

add_library(ant ${LIBANT_SOURCE_FILES})

if (BUILD_TEST_CASES)
    add_subdirectory(test_cases)
endif ()

target_include_directories(ant PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

set_target_properties(ant
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_LIBDIR}")