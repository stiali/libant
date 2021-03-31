function(TEST_FUNCTION project_name)
    file(GLOB_RECURSE TEST_SRC ${project_name}.cpp)

    if (UNIX AND (NOT APPLE))
        #Set rpath
        set(CMAKE_INSTALL_RPATH ".")
        set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    endif (UNIX AND (NOT APPLE))

    add_executable(${project_name} ${TEST_SRC})
    target_link_libraries(${project_name}
            ant)

    set_target_properties(${project_name} PROPERTIES OUTPUT_NAME_DEBUG "${project_name}_d")
    set_target_properties(${project_name} PROPERTIES
            FOLDER "test_cases"
            ARCHIVE_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}
            RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}
            LIBRARY_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
    add_test(NAME ${project_name} COMMAND ${project_name} WORKING_DIRECTORY ${BIN_OUTPUT_DIR})
endfunction(TEST_FUNCTION)

set(UNIT_TESTS test_buffer_pool)

foreach (test_index ${UNIT_TESTS})
    TEST_FUNCTION(${test_index})
endforeach ()