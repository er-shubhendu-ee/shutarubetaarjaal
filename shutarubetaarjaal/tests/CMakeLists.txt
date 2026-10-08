add_executable(shutarubetaarjaal_test
    test_main.c
)

target_link_libraries(shutarubetaarjaal_test
    PRIVATE
        shutarubetaarjaal
)

target_include_directories(shutarubetaarjaal_test
    PRIVATE
        "${CMAKE_SOURCE_DIR}"
)

set_target_properties(shutarubetaarjaal_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

foreach(CONFIG Debug Release RelWithDebInfo MinSizeRel)
    set_target_properties(shutarubetaarjaal_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "${CMAKE_BINARY_DIR}"
    )
endforeach()

add_test(
    NAME shutarubetaarjaal_test
    COMMAND shutarubetaarjaal_test
)