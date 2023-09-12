cmake_minimum_required(VERSION 3.22)
project(move_generator)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES position.cpp undo_info.cpp move.cpp)

#add_library(move_gen STATIC ${SOURCE_FILES})
add_executable(test test.cpp ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -Wextra")
