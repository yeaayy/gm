cmake_minimum_required(VERSION 3.15)
project(GM)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options("$<BUILD_INTERFACE:-Wall;-ggdb>")

add_subdirectory(common)
add_subdirectory(math)
add_subdirectory(gl)
add_subdirectory(utils)
add_subdirectory(pfa)
add_subdirectory(glview)

add_executable(run)
add_subdirectory(src)
