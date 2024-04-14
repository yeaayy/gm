
if(NOT TARGET common)
    add_subdirectory(common)
endif()

if(NOT TARGET utils)
    add_subdirectory(utils)
endif()

add_library(pfa STATIC
src/path_finder.cpp
src/pfa_simple.cpp
)

target_include_directories(pfa PUBLIC src)
target_link_libraries(pfa PUBLIC utils)