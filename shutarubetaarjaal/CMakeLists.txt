cmake_minimum_required(VERSION 3.16)

project(
    SHUTARUBETAARJAAL
    VERSION 0.1.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Build options
# ---------------------------------------------------------------------------

option(TEST "Build tests instead of the console application" OFF)

# ---------------------------------------------------------------------------
# Build directories
# ---------------------------------------------------------------------------

set(SBJ_BUILD_DIR
    "${CMAKE_BINARY_DIR}"
)

set(SBJ_ARTIFACT_DIR
    "${CMAKE_BINARY_DIR}/artifacts"
)

file(MAKE_DIRECTORY "${SBJ_ARTIFACT_DIR}")

# ---------------------------------------------------------------------------
# Core library
# ---------------------------------------------------------------------------

add_library(sbj STATIC
    sbj.c
)

target_include_directories(sbj
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Keep the static library directly under build/
set_target_properties(sbj PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${SBJ_BUILD_DIR}"
)

# Multi-config generators such as Visual Studio
foreach(CONFIG Debug Release RelWithDebInfo MinSizeRel)
    set_target_properties(sbj PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_${CONFIG} "${SBJ_BUILD_DIR}"
    )
endforeach()

# ---------------------------------------------------------------------------
# Console application / tests
# ---------------------------------------------------------------------------

if(TEST)

    message(STATUS "SHUTARUBETAARJAAL: TEST build")

    enable_testing()

    add_subdirectory(tests)

else()

    message(STATUS "SHUTARUBETAARJAAL: CONSOLE build")

    add_executable(sbj_win_console
        port/win-console/main.c
    )

    target_include_directories(sbj_win_console
        PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}"
            "${CMAKE_CURRENT_SOURCE_DIR}/port/win-console"
    )

    target_link_libraries(sbj_win_console
        PRIVATE
            sbj
    )

    # Keep executable directly under build/
    set_target_properties(sbj_win_console PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${SBJ_BUILD_DIR}"
    )

    # Multi-config generators such as Visual Studio
    foreach(CONFIG Debug Release RelWithDebInfo MinSizeRel)
        set_target_properties(sbj_win_console PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "${SBJ_BUILD_DIR}"
        )
    endforeach()

endif()