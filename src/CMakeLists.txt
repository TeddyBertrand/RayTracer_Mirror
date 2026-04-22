add_subdirectory(core)
add_subdirectory(materials)
add_subdirectory(lights)
add_subdirectory(primitives)
add_subdirectory(skies)

add_executable(raytracer main.cpp)

target_include_directories(raytracer PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer PRIVATE
    -W
    -Wall
    -Wextra
)

target_link_libraries(raytracer PRIVATE
    raytracer_core
)
