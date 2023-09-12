cmake_minimum_required(VERSION 3.22)
project(AI)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(move_generator)

set(SOURCE_FILES utilities.cpp FCNN.cpp CNN.cpp FClayer.cpp ConvLayer.cpp PoolingLayer.cpp Lens.cpp monte_carlo_tree_search.cpp monte_carlo_tree_search.h)

#add_executable(AI main.cpp ${SOURCE_FILES})
add_library(AI STATIC ${SOURCE_FILES})

target_link_libraries(AI PRIVATE move_gen)
target_include_directories(AI PUBLIC C:/includes/eigen-3.4.0/)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -Wextra")
