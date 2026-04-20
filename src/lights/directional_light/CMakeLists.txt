add_library(raytracer_directional_light SHARED
    DirectionalLight.cpp
)

target_include_directories(raytracer_directional_light PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_directional_light PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_directional_light PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_directional_light"
)
