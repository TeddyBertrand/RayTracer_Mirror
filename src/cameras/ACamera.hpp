#pragma once

#include "components/ICamera.hpp"
#include "math/MathUtils.hpp"
#include "math/Matrix.hpp"
#include "math/Vector3D.hpp"

namespace Raytracer {

class ACamera : public ICamera {
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

    ACamera(
        const Point3D& origin, const Vector3D& rotation, const Vector3D& vup, int width, int height)
        : _origin(origin), _width(width), _height(height) {
        double pitch = Math::degreesToRadians(rotation.x);
        double yaw = Math::degreesToRadians(rotation.y);
        double roll = Math::degreesToRadians(rotation.z);

        _forward = Vector3D(cos(pitch) * sin(yaw), sin(pitch), -cos(pitch) * cos(yaw)).normalized();

        setupBase(vup);

        if (roll != 0) {
            Vector3D old_right = _right;
            Vector3D old_up = _up;

            _right = old_right * cos(roll) + old_up * sin(roll);
            _up = old_up * cos(roll) - old_right * sin(roll);
        }
    }

private:
    void setupBase(const Vector3D& vup) {
        const Matrix basis = Matrix::lookAt(_origin, _origin + _forward, vup);

        _right = {basis.m[0][0], basis.m[0][1], basis.m[0][2]};
        _up = {basis.m[1][0], basis.m[1][1], basis.m[1][2]};
        _forward = {-basis.m[2][0], -basis.m[2][1], -basis.m[2][2]};
    }
};

} // namespace Raytracer
