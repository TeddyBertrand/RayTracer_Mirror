add_library(raytracer_lightmaterial SHARED
    LightMaterial.cpp
)

target_link_libraries(raytracer_lightmaterial PRIVATE raytracer_materials_common)

target_include_directories(raytracer_lightmaterial PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_lightmaterial PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_lightmaterial PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_lightmaterial"
)
