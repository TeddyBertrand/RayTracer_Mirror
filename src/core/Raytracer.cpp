#include "Raytracer.hpp"
#include "core/camera/PerspectiveCamera.hpp"
#include "primitives/sphere/Sphere.hpp"
#include "core/image/Image.hpp"
#include "materials/lambertian/Lambertian.hpp"
#include "lights/point_light/PointLight.hpp"
#include "skies/atmospheric_sky/AtmosphericSky.hpp"
#include "skies/empty_sky/EmptySky.hpp"
#include "skies/galaxy_sky/GalaxySky.hpp"

namespace Raytracer
{

Raytracer::Raytracer(int argc, const char **argv)
{
    (void)argc;
    (void)argv;
}

void Raytracer::run()
{
    if (_exitCode != SUCCESS_STATUS)
        return;

    FrameBuffer frameBuffer;
    PerspectiveCamera camera(Vector3D::zero(), Vector3D::zero(), 60, 16.0/9.0, 1920, 1080);
    Image _image(camera.getWidth(), camera.getHeight());
    std::shared_ptr<IMaterial> lambertian = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
    
    auto sky = std::make_unique<AtmosphericSky>(Color(0.5, 0.7, 1.0), Color(1.0, 1.0, 1.0));
    auto emptySky = std::make_unique<EmptySky>();
    auto galaxySky = std::make_unique<GalaxySky>();
    auto sphere = std::make_shared<Sphere>(Vector3D(0, 0, -3), 0.5, lambertian);
    auto sphere2 = std::make_shared<Sphere>(Vector3D(1.2, 0, -3), 0.5, lambertian);
    auto light = std::make_shared<PointLight>(Vector3D(1, 1.4, -2), Color(1, 1, 1), .5);
    _scene.setSky(std::move(galaxySky));
    _scene.addPrimitive(sphere);
    _scene.addPrimitive(sphere2);
    _scene.addLight(light);
    _scene.setBackgroundColor(Color(0, 0, 0));

    _renderer.render(camera, _scene, frameBuffer);
    _image.drawFromBuffer(frameBuffer);
}

} // namespace Raytracer
