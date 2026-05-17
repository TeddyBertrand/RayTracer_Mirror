add_library(raytracer_ambient_light SHARED
    AmbientLight.cpp
)

target_include_directories(raytracer_ambient_light PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_ambient_light PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_ambient_light PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_ambient_light"
)
