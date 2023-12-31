cmake_minimum_required(VERSION 3.22)
project(AI)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(move_generator)

include(FetchContent)
FetchContent_Declare(
  Eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG master
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE)
set(EIGEN_BUILD_DOC OFF)
# note: To disable eigen tests,
# you should put this code in a add_subdirectory to avoid to change
# BUILD_TESTING for your own project too since variables are directory
# scoped
set(BUILD_TESTING OFF)
set(EIGEN_BUILD_PKGCONFIG OFF)
set( OFF)
FetchContent_MakeAvailable(Eigen)


set(SOURCE_FILES utilities.cpp FCNN.cpp CNN.cpp FCLayer.cpp ConvLayer.cpp PoolingLayer.cpp Lens.cpp monte_carlo_tree_search.cpp monte_carlo_tree_search.h)

#add_executable(AI main.cpp ${SOURCE_FILES})
add_library(AI STATIC ${SOURCE_FILES})

target_link_libraries(AI PRIVATE move_gen)
target_link_libraries (AI PUBLIC Eigen3::Eigen)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -Wextra")

