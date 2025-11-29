cmake_minimum_required(VERSION 3.22.1)
project(xdl)

set(CMAKE_C_STANDARD 17)

file(GLOB XDL_SRC *.c)
add_library(xdl STATIC ${XDL_SRC})
target_compile_options(xdl PRIVATE -Werror)
target_include_directories(xdl PUBLIC include)

if(USEASAN)
    target_compile_options(xdl PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(xdl PRIVATE -fsanitize=address)
else()
    target_compile_options(xdl PRIVATE -Os -ffunction-sections -fdata-sections)
    target_link_options(xdl PRIVATE -Os -Wl,--exclude-libs,ALL -Wl,--gc-sections -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/xdl.map.txt)
endif()

if((${ANDROID_ABI} STREQUAL "arm64-v8a") OR (${ANDROID_ABI} STREQUAL "x86_64"))
    target_link_options(xdl PRIVATE "-Wl,-z,max-page-size=16384")
endif()
