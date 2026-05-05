add_library(raytracer_infinite_cylinder SHARED
    InfiniteCylinder.cpp
)

target_include_directories(raytracer_infinite_cylinder PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_infinite_cylinder PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_infinite_cylinder PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_infinite_cylinder"
)
