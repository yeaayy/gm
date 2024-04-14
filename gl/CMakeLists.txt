
if(NOT TARGET common)
    add_subdirectory(common)
endif()

add_library(mgl STATIC
    src/shader_program.cpp
    src/texture_loader.cpp
)

if(ANDROID)
    target_link_libraries(mgl PUBLIC -lGLESv2)
elseif(UNIX)
    target_link_libraries(mgl PUBLIC -lX11 -lGL)
endif()

target_include_directories(mgl PUBLIC src)
target_link_libraries(mgl PUBLIC common)
target_link_libraries(mgl PUBLIC -lpng)