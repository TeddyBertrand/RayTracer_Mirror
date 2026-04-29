add_library(raytracer_math INTERFACE)

target_include_directories(raytracer_math INTERFACE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_math INTERFACE
    -W
    -Wall
    -Wextra
)
