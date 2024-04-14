
if(NOT TARGET common)
    add_subdirectory(common)
endif()

add_library(utils STATIC
    src/direction.cpp
    src/frame_counter.cpp
    src/state.cpp
    src/timer.cpp
    src/updatable.cpp
)

target_include_directories(utils PUBLIC src)
target_link_libraries(utils PUBLIC common)