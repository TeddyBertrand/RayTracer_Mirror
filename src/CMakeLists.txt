add_subdirectory(core)
add_subdirectory(lights)
add_subdirectory(materials)
add_subdirectory(primitives)
add_subdirectory(skies)

add_executable(raytracer main.cpp)
target_link_libraries(raytracer PRIVATE raytracer_core)

set_target_properties(raytracer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)