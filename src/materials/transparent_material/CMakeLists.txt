add_library(raytracer_transparent_material SHARED
    TransparentMaterial.cpp
)

if(TARGET sfml-graphics)
    target_link_libraries(raytracer_transparent_material PRIVATE raytracer_materials_common sfml-graphics sfml-system)
elseif(TARGET SFML::Graphics)
    target_link_libraries(raytracer_transparent_material PRIVATE raytracer_materials_common SFML::Graphics SFML::System)
else()
    target_link_libraries(raytracer_transparent_material PRIVATE raytracer_materials_common ${SFML_LIBRARIES})
endif()

target_include_directories(raytracer_transparent_material PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_transparent_material PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_transparent_material PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_transparent_material"
)
