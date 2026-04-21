#include "Raytracer.hpp"
#include "core/camera/PerspectiveCamera.hpp"
#include "primitives/sphere/Sphere.hpp"
#include "core/image/Image.hpp"

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
    PerspectiveCamera camera(Vector3D::zero(), Vector3D::zero(), 75, 16.0/9.0, 1920, 1080);
    Image _image(camera.getWidth(), camera.getHeight());

    auto world = std::make_shared<Sphere>(Vector3D(0, 0, -1), 0.5);

    _renderer.render(camera, *world, frameBuffer);
    _image.drawFromBuffer(frameBuffer);
}

} // namespace Raytracer
