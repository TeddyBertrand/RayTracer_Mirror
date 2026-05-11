add_library(raytracer_fractal SHARED
	strategy/menger_sponge/MengerSpongeStrategy.cpp
	strategy/mandelbulb/MandelbulbStrategy.cpp
	strategy/moebius_strip/MoebiusStrategy.cpp

	Fractal.cpp
	FractalStrategyFactory.cpp
)

target_include_directories(raytracer_fractal PRIVATE
    ${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_fractal PRIVATE
	-W
	-Wall
	-Wextra
)

set_target_properties(raytracer_fractal PROPERTIES
	PREFIX ""
	OUTPUT_NAME "raytracer_fractal"
)
