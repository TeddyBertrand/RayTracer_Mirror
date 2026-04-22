add_library(raytracer_core STATIC
    Raytracer.cpp
    camera/PerspectiveCamera.cpp
    renderer/Renderer.cpp
    scene/Scene.cpp
    scene/PrimitiveList.cpp
    image/Image.cpp
    ../primitives/sphere/Sphere.cpp
    ../materials/lambertian/Lambertian.cpp
    ../lights/point_light/PointLight.cpp
)

target_include_directories(raytracer_core PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_core PRIVATE
    -W
    -Wall
    -Wextra
)
