
add_library(common STATIC)

if(ANDROID)
    target_sources(common PUBLIC src/log.android.c)
elseif(UNIX)
    target_sources(common PUBLIC src/log.linux.c)
endif()

target_include_directories(common PUBLIC include)