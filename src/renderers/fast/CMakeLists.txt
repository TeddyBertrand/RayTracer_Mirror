add_library(raytracer_fast_renderer SHARED
    FastRenderer.cpp
)

target_include_directories(raytracer_fast_renderer PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(raytracer_fast_renderer PRIVATE
    raytracer_core
)

target_compile_options(raytracer_fast_renderer PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_fast_renderer PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_fast_renderer"
)
