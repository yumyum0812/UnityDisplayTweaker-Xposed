project(udt-native LANGUAGES CXX)

add_library(${PROJECT_NAME} SHARED
        src/lib_main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        android
        log
        xdl-static
)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_STRIP} --strip-all $<TARGET_FILE:${PROJECT_NAME}>
    )
endif()