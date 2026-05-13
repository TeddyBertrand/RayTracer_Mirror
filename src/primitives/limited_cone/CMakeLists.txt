add_library(raytracer_limited_cone SHARED
    LimitedCone.cpp
)

target_include_directories(raytracer_limited_cone PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_limited_cone PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_limited_cone PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_limited_cone"
)