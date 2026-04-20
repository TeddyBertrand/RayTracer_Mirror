add_library(raytracer_sphere SHARED
	Sphere.cpp
)

target_include_directories(raytracer_sphere PRIVATE
    ${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_sphere PRIVATE
	-W
	-Wall
	-Wextra
)

set_target_properties(raytracer_sphere PROPERTIES
	PREFIX ""
	OUTPUT_NAME "raytracer_sphere"
)
