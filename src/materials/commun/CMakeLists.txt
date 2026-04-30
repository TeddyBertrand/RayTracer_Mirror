add_library(raytracer_materials_common STATIC
    bsdf/lambertian/LambertianBSDF.cpp
    bsdf/emissive/EmissiveBSDF.cpp
)

find_package(SFML COMPONENTS Graphics System REQUIRED)
target_link_libraries(raytracer_materials_common PUBLIC SFML::Graphics SFML::System)

set_target_properties(raytracer_materials_common PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(raytracer_materials_common PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_materials_common PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_materials_common PROPERTIES
    OUTPUT_NAME "raytracer_materials_common"
)
