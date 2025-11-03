cmake_minimum_required(VERSION 3.11)
project(RaylibHotReload LANGUAGES C)

# Generate compile_commands.json for editors
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fetch raylib 5.5
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# Make raylib PIC for shared lib linking
set_property(TARGET raylib PROPERTY POSITION_INDEPENDENT_CODE ON)

# Main executable (links raylib statically for persistent context)
add_executable(main src/main.c src/loader.c)
target_link_libraries(main PRIVATE raylib)
target_include_directories(main PRIVATE 
    include 
    ${raylib_SOURCE_DIR}/src
)  # For external.h if needed
# target_compile_definitions(main PRIVATE HOT_RELOAD)  # Enable hot-reload mode

# Game DLL (shared lib, hot-reloadable)
add_library(game SHARED src/game.c)
target_link_libraries(game PRIVATE raylib)
target_include_directories(game PRIVATE 
    include 
    ${raylib_SOURCE_DIR}/src
)  # For external.h if needed
# target_compile_definitions(logic PRIVATE BUILD_DLL HOT_RELOAD _LOGIC_EXTERNAL) # Export for DLL  # Custom flag for extern
set_target_properties(game PROPERTIES PREFIX "" SUFFIX ".dll" OUTPUT_NAME "game")
target_link_libraries(game PRIVATE -Wl,--export-all-symbols)  # Force export if needed