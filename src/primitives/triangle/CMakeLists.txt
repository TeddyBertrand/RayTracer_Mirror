add_library(raytracer_triangle SHARED
        Triangle.cpp
)

target_include_directories(raytracer_triangle PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_triangle PRIVATE
        -W
        -Wall
        -Wextra
)

set_target_properties(raytracer_triangle PROPERTIES
        PREFIX ""
        OUTPUT_NAME "raytracer_triangle"
)
