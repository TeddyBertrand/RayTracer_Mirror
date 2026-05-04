add_library(raytracer_cone SHARED
	Cone.cpp
)

target_include_directories(raytracer_cone PRIVATE
    ${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_cone PRIVATE
	-W
	-Wall
	-Wextra
)

set_target_properties(raytracer_cone PROPERTIES
	PREFIX ""
	OUTPUT_NAME "raytracer_cone"
)
