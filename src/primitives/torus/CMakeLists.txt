add_library(raytracer_torus SHARED
	Torus.cpp
)

target_include_directories(raytracer_torus PRIVATE
    ${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_torus PRIVATE
	-W
	-Wall
	-Wextra
)

set_target_properties(raytracer_torus PROPERTIES
	PREFIX ""
	OUTPUT_NAME "raytracer_torus"
)
