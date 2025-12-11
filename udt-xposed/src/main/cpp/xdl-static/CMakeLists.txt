project(xdl-static LANGUAGES C)

set(CMAKE_C_STANDARD 17)

file(GLOB XDL_SRC src/*.c)
add_library(${PROJECT_NAME} STATIC ${XDL_SRC})
target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
target_include_directories(${PROJECT_NAME} PUBLIC include)

if(USEASAN)
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Os -ffunction-sections -fdata-sections)
    target_link_options(${PROJECT_NAME} PRIVATE -Os -Wl,--exclude-libs,ALL -Wl,--gc-sections -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/xdl.map.txt)
endif()

if((${ANDROID_ABI} STREQUAL "arm64-v8a") OR (${ANDROID_ABI} STREQUAL "x86_64"))
    target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-z,max-page-size=16384")
endif()
