#include "Raytracer.hpp"
#include "core/camera/PerspectiveCamera.hpp"
#include "primitives/sphere/Sphere.hpp"
#include "core/image/Image.hpp"
#include "materials/lambertian/Lambertian.hpp"

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

    auto sphere = std::make_shared<Sphere>(Vector3D(0, 0, -3), 0.5, lambertian);
    auto sphere2 = std::make_shared<Sphere>(Vector3D(1.2, 0, -3), 0.5, lambertian);
    _scene.addPrimitive(sphere);
    _scene.addPrimitive(sphere2);

    _renderer.render(camera, _scene, frameBuffer);
    _image.drawFromBuffer(frameBuffer);
}

} // namespace Raytracer
