include(FetchContent)

find_package(SFML 3 QUIET COMPONENTS Graphics System)

if(NOT SFML_FOUND)
    message(STATUS "SFML not found on current machine, fetching it now...")
    FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
    )
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_BACKUP ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/_deps/lib)
    FetchContent_MakeAvailable(sfml)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_BACKUP})
endif()

add_library(raytracer_materials_common STATIC
    bsdf/lambertian/LambertianBSDF.cpp
    bsdf/emissive/EmissiveBSDF.cpp
)

set_target_properties(raytracer_materials_common PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    OUTPUT_NAME "raytracer_materials_common"
)

target_include_directories(raytracer_materials_common PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

if(TARGET sfml-graphics)
    target_link_libraries(raytracer_materials_common PRIVATE
        sfml-graphics
        sfml-system
    )
elseif(TARGET SFML::Graphics)
    target_link_libraries(raytracer_materials_common PRIVATE
        SFML::Graphics
        SFML::System
    )
else()
    target_include_directories(raytracer_materials_common PRIVATE ${SFML_INCLUDE_DIR})
    target_link_libraries(raytracer_materials_common PRIVATE ${SFML_LIBRARIES})
endif()

target_compile_options(raytracer_materials_common PRIVATE
    -W
    -Wall
    -Wextra
    -fno-gnu-unique
)