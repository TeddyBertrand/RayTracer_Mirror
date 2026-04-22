add_library(raytracer_atmospheric_sky SHARED
	AtmosphericSky.cpp
)

target_include_directories(raytracer_atmospheric_sky PRIVATE
    ${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_atmospheric_sky PRIVATE
	-W
	-Wall
	-Wextra
)

set_target_properties(raytracer_atmospheric_sky PROPERTIES
	PREFIX ""
	OUTPUT_NAME "raytracer_atmospheric_sky"
)
