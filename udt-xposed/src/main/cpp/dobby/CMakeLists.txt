project(dobby)

add_library(dobby STATIC IMPORTED GLOBAL)

set_target_properties(dobby PROPERTIES
    IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/${CMAKE_ANDROID_ARCH_ABI}/libdobby.a
)

target_include_directories(dobby INTERFACE
    include
)