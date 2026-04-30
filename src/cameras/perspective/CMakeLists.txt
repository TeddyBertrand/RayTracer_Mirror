add_library(raytracer_perspective_camera SHARED
    PerspectiveCamera.cpp
)


target_include_directories(raytracer_perspective_camera PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_perspective_camera PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_perspective_camera PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_perspective_camera"
)
