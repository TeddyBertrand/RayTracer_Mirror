add_library(raytracer_box SHARED
	Box.cpp
)

target_include_directories(raytracer_box PRIVATE
    ${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_box PRIVATE
	-W
	-Wall
	-Wextra
)

set_target_properties(raytracer_box PROPERTIES
	PREFIX ""
	OUTPUT_NAME "raytracer_box"
)
