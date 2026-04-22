add_library(raytracer_lambertian SHARED
    Lambertian.cpp
)

target_include_directories(raytracer_lambertian PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_lambertian PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_lambertian PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_lambertian"
)
