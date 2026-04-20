#pragma once

#include "math/Vector3D.hpp"
#include "components/ICamera.hpp"

namespace Raytracer
{

class ACamera : public ICamera
{
public:
    int getWidth() const override { return _width; }
    int getHeight() const override { return _height; }

protected:
    Point3D _origin;
    Vector3D _forward;
    Vector3D _right;
    Vector3D _up;
    int _width;
    int _height;

    ACamera(const Point3D& origin, const Vector3D& rotation, const Vector3D& vup, int width, int height) 
        : _origin(origin), _width(width), _height(height)
    {
        double pitch = rotation.x * M_PI / 180.0;
        double yaw   = rotation.y * M_PI / 180.0;

        _forward = Vector3D(
            cos(pitch) * sin(yaw),
            sin(pitch),
            -cos(pitch) * cos(yaw)
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