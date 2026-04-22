add_library(raytracer_empty_sky SHARED
	EmptySky.cpp
)

target_include_directories(raytracer_empty_sky PRIVATE
    ${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_empty_sky PRIVATE
	-W
	-Wall
	-Wextra
)

set_target_properties(raytracer_empty_sky PROPERTIES
	PREFIX ""
	OUTPUT_NAME "raytracer_empty_sky"
)
