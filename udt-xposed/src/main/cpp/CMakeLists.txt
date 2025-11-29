cmake_minimum_required(VERSION 3.22.1)

project("udt-native")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_VISIBILITY_PRESET default)
else()
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
endif()


add_subdirectory(dobby)
add_subdirectory(xdl)

add_library(${CMAKE_PROJECT_NAME} SHARED
        src/lib_main.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        dobby
        xdl
)