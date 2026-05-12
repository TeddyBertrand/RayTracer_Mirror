add_library(raytracer_default_renderer SHARED
    Renderer.cpp
)

target_include_directories(raytracer_default_renderer PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(raytracer_default_renderer PRIVATE
    raytracer_core
)

target_compile_options(raytracer_default_renderer PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_default_renderer PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_default_renderer"
)
