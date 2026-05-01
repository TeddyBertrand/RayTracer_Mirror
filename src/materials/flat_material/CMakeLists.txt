add_library(raytracer_flatmaterial SHARED
    FlatMaterial.cpp
)

target_link_libraries(raytracer_flatmaterial PRIVATE raytracer_materials_common)

target_include_directories(raytracer_flatmaterial PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_flatmaterial PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_flatmaterial PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_flatmaterial"
)
