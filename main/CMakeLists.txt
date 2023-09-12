cmake_minimum_required(VERSION 3.22)
project(chess_ai)

set(CMAKE_CXX_STANDARD 20)

cmake_minimum_required(VERSION 3.22)
project(UI)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(AI)

find_package(raylib 4.2.0 QUIET)

if (NOT raylib_FOUND)
    include(FetchContent)

    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/4.2.0.tar.gz
    )

    FetchContent_GetProperties(raylib)

    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

set(SOURCE_FILES main.cpp game.cpp menu.cpp Button.cpp)

add_executable(chess ${SOURCE_FILES})
target_link_libraries(chess PRIVATE AI)
target_link_libraries(chess PRIVATE raylib)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -Wextra")