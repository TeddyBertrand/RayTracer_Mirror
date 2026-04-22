add_library(raytracer_point_light SHARED
    PointLight.cpp
)

target_include_directories(raytracer_point_light PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_point_light PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_point_light PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_point_light"
)
