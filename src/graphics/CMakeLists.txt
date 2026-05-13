set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins/graphics)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins/graphics)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/plugins/graphics)

add_library(raytracer_sfml_display SHARED
    SFMLDisplay.cpp
)

target_include_directories(raytracer_sfml_display PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(raytracer_sfml_display PRIVATE
    raytracer_core
    sfml-graphics
    sfml-window
    sfml-system
)

target_compile_options(raytracer_sfml_display PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_sfml_display PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_sfml_display"
)
