#pragma once

#include "math/Vector3D.hpp"

namespace Raytracer
{

class Ray
{
public:
    constexpr Ray() noexcept {}
    constexpr Ray(const Point3D& origin, const Vector3D& direction) noexcept 
        : _orig(origin), _dir(direction) {}

    [[nodiscard]] constexpr Point3D origin() const noexcept { return _orig; }
    [[nodiscard]] constexpr Vector3D direction() const noexcept { return _dir; }

    [[nodiscard]] constexpr Point3D at(double t) const noexcept {
        return _orig + t * _dir;
    }

private:
    Point3D _orig;
    Vector3D _dir;
};

} // namespace Raytracer
