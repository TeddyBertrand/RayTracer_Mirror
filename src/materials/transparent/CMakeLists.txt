add_library(raytracer_transparent SHARED
    Transparent.cpp
)

target_include_directories(raytracer_transparent PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_transparent PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_transparent PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_transparent"
)
