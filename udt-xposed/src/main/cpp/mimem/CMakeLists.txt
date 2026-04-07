project(mimem LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC
        src/mem_map.cpp
        src/mem_patch.cpp
        src/mem_search.cpp
        src/safe_mem.cpp
        src/elf_inspect.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE
        android
        log
        dl
)