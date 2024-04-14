
if(NOT TARGET mgl)
    add_subdirectory(gl)
endif()

if(NOT TARGET common)
    add_subdirectory(common)
endif()

add_library(glview STATIC
src/image_view.cpp
src/keymap.cpp
src/main.all.cpp
src/rectf.cpp
src/text_view.cpp
src/view.cpp
)


if(ANDROID)
    target_sources(glview PUBLIC src/main.android.cpp)
elseif(UNIX)
    target_sources(glview PUBLIC src/main.linux.cpp)
endif()

target_include_directories(glview PUBLIC include)
target_link_libraries(glview PUBLIC mgl)