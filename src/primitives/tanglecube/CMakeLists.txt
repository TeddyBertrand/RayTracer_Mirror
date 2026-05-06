add_library(raytracer_tanglecube SHARED
    Tanglecube.cpp
)

target_include_directories(raytracer_tanglecube PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_tanglecube PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_tanglecube PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_tanglecube"
)
