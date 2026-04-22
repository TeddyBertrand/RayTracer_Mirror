#pragma once

#include "ACamera.hpp"

namespace Raytracer
{

class PerspectiveCamera : public ACamera
{
public:
    PerspectiveCamera(const Point3D& origin, const Vector3D& rotation, double fov, double aspect_ratio, int width, int height);

    Ray getRay(double u, double v) const override;

private:
    Vector3D _horizontal;
    Vector3D _vertical;
    Point3D _lower_left_corner;

    void _initialize(double fov, double aspect_ratio);
};

} // namespace Raytracer