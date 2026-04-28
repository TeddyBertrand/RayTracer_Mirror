find_package(SFML 2 QUIET COMPONENTS graphics window system)

add_library(raytracer_texture SHARED
    Texture.cpp
)

target_include_directories(raytracer_texture PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(raytracer_texture PRIVATE
    sfml-graphics
)

target_compile_options(raytracer_texture PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_texture PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_texture"
)
