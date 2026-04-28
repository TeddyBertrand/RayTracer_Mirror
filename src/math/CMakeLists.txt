add_library(raytracer_math STATIC
    Matrix.cpp
)

target_include_directories(raytracer_math PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_math PRIVATE
    -W
    -Wall
    -Wextra
)
