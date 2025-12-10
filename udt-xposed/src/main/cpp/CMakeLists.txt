cmake_minimum_required(VERSION 3.22.1)

project(UDT-Xposed-Native LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_VISIBILITY_PRESET default)
else()
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
endif()

add_subdirectory(xdl-static)
add_subdirectory(udt-native)