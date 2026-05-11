add_library(raytracer_spot_light SHARED
    SpotLight.cpp
)

target_include_directories(raytracer_spot_light PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_spot_light PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_spot_light PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_spot_light"
)
