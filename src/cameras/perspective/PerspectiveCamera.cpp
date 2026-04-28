#include "PerspectiveCamera.hpp"
#include "factory/CameraFactory.hpp"

#include <cmath>

namespace Raytracer {

extern "C" {
ICamera* createPlugin(const ISetting& settings) {
    auto pos = settings.getVector("position");
    auto lookAt = settings.getVector("look_at");
    float fov = settings.getFloat("fieldOfView");

    int width = settings.getInt("width");
    int height = settings.getInt("height");
    double aspect_ratio = static_cast<double>(width) / height;

    return new PerspectiveCamera(pos, lookAt, fov, aspect_ratio, width, height);
}
}

PerspectiveCamera::PerspectiveCamera(const Point3D& origin,
                                     const Vector3D& rotation,
                                     double fov,
                                     double aspect_ratio,
                                     int width,
                                     int height)
    : ACamera(origin, rotation, Vector3D::up(), width, height) {
    initialize(fov, aspect_ratio);
}

Ray PerspectiveCamera::getRay(double u, double v) const {
    Vector3D target_point = _lower_left_corner + (u * _horizontal) + (v * _vertical);

    return Ray(_origin, target_point - _origin);
}

void PerspectiveCamera::initialize(double fov, double aspect_ratio) {
    double theta = fov * M_PI / 180.0;
    double h = std::tan(theta / 2.0);

    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;

    _horizontal = viewport_width * _right;
    _vertical = viewport_height * _up;

    _lower_left_corner = _origin + _forward - (_horizontal / 2.0) - (_vertical / 2.0);
}

} // namespace Raytracer
