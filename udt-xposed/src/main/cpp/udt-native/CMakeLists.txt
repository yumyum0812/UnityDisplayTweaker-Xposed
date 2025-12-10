cmake_minimum_required(VERSION 3.22.1)

project(udt-native LANGUAGES CXX)

add_library(${PROJECT_NAME} SHARED
        src/lib_main.cpp
)

target_link_libraries(${PROJECT_NAME}
        android
        log
        xdl-static
)