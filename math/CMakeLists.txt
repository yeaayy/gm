add_library(math STATIC
src/camera.cpp
src/ivec2.cpp
src/mat4.c
src/ray3.cpp
)

target_include_directories(math PUBLIC include)