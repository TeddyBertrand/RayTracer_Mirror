add_library(raytracer_flat_color SHARED
    FlatColor.cpp
)

target_include_directories(raytracer_flat_color PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_flat_color PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_flat_color PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_flat_color"
)
