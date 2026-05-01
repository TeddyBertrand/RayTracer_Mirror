add_library(raytracer_cylinder SHARED
	Cylinder.cpp
)

target_include_directories(raytracer_cylinder PRIVATE
    ${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_cylinder PRIVATE
	-W
	-Wall
	-Wextra
)

set_target_properties(raytracer_cylinder PROPERTIES
	PREFIX ""
	OUTPUT_NAME "raytracer_cylinder"
)
