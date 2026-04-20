add_library(raytracer_plane SHARED
	Plane.cpp
)

target_include_directories(raytracer_plane PRIVATE
    ${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_plane PRIVATE
	-W
	-Wall
	-Wextra
)

set_target_properties(raytracer_plane PROPERTIES
	PREFIX ""
	OUTPUT_NAME "raytracer_plane"
)
