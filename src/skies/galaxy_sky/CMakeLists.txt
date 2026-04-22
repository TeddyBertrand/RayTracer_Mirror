add_library(raytracer_galaxy_sky SHARED
	GalaxySky.cpp
)

target_include_directories(raytracer_galaxy_sky PRIVATE
    ${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_galaxy_sky PRIVATE
	-W
	-Wall
	-Wextra
)

set_target_properties(raytracer_galaxy_sky PROPERTIES
	PREFIX ""
	OUTPUT_NAME "raytracer_galaxy_sky"
)
