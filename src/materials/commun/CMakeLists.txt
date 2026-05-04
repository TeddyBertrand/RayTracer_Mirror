find_package(PkgConfig QUIET)

if(PKG_CONFIG_FOUND)
    pkg_check_modules(SFML sfml-graphics sfml-system)
endif()

if(NOT SFML_FOUND)
    find_package(SFML 3 COMPONENTS graphics system QUIET)
    if(NOT SFML_FOUND)
        find_package(SFML COMPONENTS graphics system REQUIRED)
    endif()
endif()

add_library(raytracer_materials_common STATIC
    bsdf/lambertian/LambertianBSDF.cpp
    bsdf/emissive/EmissiveBSDF.cpp
)

if(TARGET sfml-graphics)
    target_link_libraries(raytracer_materials_common PUBLIC sfml-graphics sfml-system)
else()
    target_link_libraries(raytracer_materials_common PUBLIC ${SFML_LIBRARIES})
    target_include_directories(raytracer_materials_common PUBLIC ${SFML_INCLUDE_DIRS})
endif()

set_target_properties(raytracer_materials_common PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    OUTPUT_NAME "raytracer_materials_common"
)

target_include_directories(raytracer_materials_common PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_materials_common PRIVATE -W -Wall -Wextra)