find_package(SFML 2 QUIET COMPONENTS graphics window system)

add_library(raytracer_core STATIC
    Raytracer.cpp
    camera/PerspectiveCamera.cpp
    renderer/Renderer.cpp
    scene/Scene.cpp
    scene/PrimitiveList.cpp
    image/Image.cpp
    ../primitives/sphere/Sphere.cpp
    ../materials/lambertian/Lambertian.cpp
    ../materials/transparent/Transparent.cpp
    ../lights/point_light/PointLight.cpp
    ../skies/atmospheric_sky/AtmosphericSky.cpp
    ../skies/empty_sky/EmptySky.cpp
    ../skies/galaxy_sky/GalaxySky.cpp
    ../materials/texture/Texture.cpp
)

target_include_directories(raytracer_core PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(raytracer_core PRIVATE
    sfml-graphics
)

target_compile_options(raytracer_core PRIVATE
    -W
    -Wall
    -Wextra
)
