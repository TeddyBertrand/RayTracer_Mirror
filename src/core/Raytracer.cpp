#include "Raytracer.hpp"
#include "core/camera/PerspectiveCamera.hpp"
#include "core/image/Image.hpp"
#include "lights/point_light/PointLight.hpp"
#include "materials/lambertian/Lambertian.hpp"
#include "materials/transparent/Transparent.hpp"
#include "primitives/sphere/Sphere.hpp"
#include "skies/atmospheric_sky/AtmosphericSky.hpp"
#include "skies/empty_sky/EmptySky.hpp"
#include "skies/galaxy_sky/GalaxySky.hpp"

namespace Raytracer {

Raytracer::Raytracer(int argc, const char** argv) {
    (void)argc;
    (void)argv;
}

void Raytracer::run() {
    if (_exitCode != SUCCESS_STATUS)
        return;

    FrameBuffer frameBuffer;
    PerspectiveCamera camera(Vector3D(0, 1.5f, 0), Vector3D(-25, 0, 0), 60, 16.0 / 9.0, 1920, 1080);
    Image _image(camera.getWidth(), camera.getHeight());
    std::shared_ptr<IMaterial> lambertianRed = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
    std::shared_ptr<IMaterial> lambertianGreen = std::make_shared<Lambertian>(Color(0.3, 0.7, 0.3));
    std::shared_ptr<IMaterial> transparent =
        std::make_shared<Transparent>(Color(0.8, 0.8, 0.8), 0.8);

    auto sky = std::make_unique<AtmosphericSky>(Color(0.5, 0.7, 1.0), Color(0.5, 0.5, 0.5));
    auto emptySky = std::make_unique<EmptySky>();
    auto galaxySky = std::make_unique<GalaxySky>();
    auto sphere = std::make_shared<Sphere>(Vector3D(0, 0, -3), 0.5, lambertianRed);
    auto sphere2 = std::make_shared<Sphere>(Vector3D(1.2, 0, -3), 0.5, lambertianRed);
    auto sphere3 = std::make_shared<Sphere>(Vector3D(-0.5, 0, -2), 0.5, transparent);
    auto bigSphere = std::make_shared<Sphere>(Vector3D(0, -100.5, -1), 100, lambertianGreen);
    auto light = std::make_shared<PointLight>(Vector3D(1, 1.4, -2), Color(1, 1, 1), .5);
    _scene.setSky(std::move(sky));
    _scene.addPrimitive(sphere);
    _scene.addPrimitive(sphere2);
    _scene.addPrimitive(sphere3);
    _scene.addPrimitive(bigSphere);
    _scene.addLight(light);
    _scene.setBackgroundColor(Color(0, 0, 0));

    _renderer.setSamples(1);
    _renderer.render(camera, _scene, frameBuffer);
    _image.drawFromBuffer(frameBuffer);
}

} // namespace Raytracer
