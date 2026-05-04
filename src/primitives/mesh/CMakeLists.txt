add_library(raytracer_mesh SHARED
	Mesh.cpp
	parser/ObjParser.cpp
	../triangle/Triangle.cpp
)

target_include_directories(raytracer_mesh PRIVATE
    ${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_mesh PRIVATE
	-W
	-Wall
	-Wextra
)

set_target_properties(raytracer_mesh PROPERTIES
	PREFIX ""
	OUTPUT_NAME "raytracer_mesh"
)
