add_executable(sbj_win_console
    main.c
)

target_include_directories(sbj_win_console
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(sbj_win_console
    PRIVATE
        sbj
)

set_target_properties(sbj_win_console PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

foreach(CONFIG Debug Release RelWithDebInfo MinSizeRel)
    set_target_properties(sbj_win_console PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "${CMAKE_BINARY_DIR}"
    )
endforeach()