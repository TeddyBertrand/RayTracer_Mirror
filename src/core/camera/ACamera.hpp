#pragma once

#include "math/Vector3D.hpp"
#include "components/ICamera.hpp"

namespace Raytracer
{

class ACamera : public ICamera
{
protected:
    Point3D _origin;
    Vector3D _forward;
    Vector3D _right;
    Vector3D _up;

    ACamera(const Point3D& origin, const Vector3D& rotation, const Vector3D& vup) 
        : _origin(origin) 
    {
        double pitch = rotation.x * M_PI / 180.0;
        double yaw   = rotation.y * M_PI / 180.0;

        _forward = Vector3D(
            std::cos(pitch) * std::sin(yaw),
            std::sin(pitch),
            std::cos(pitch) * std::cos(yaw)
        ).normalized();

        _setupBase(vup);
    }

private:
    void _setupBase(const Vector3D& vup) {
        if (std::abs(_forward.dot(vup)) > 0.999) {
            _right = _forward.cross(Vector3D::unit_z()).normalized();
        } else {
            _right = _forward.cross(vup).normalized();
        }
        _up = _right.cross(_forward);
    }
};

} // namespace Raytracer