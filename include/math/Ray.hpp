#pragma once

#include "math/Vector3D.hpp"

namespace Raytracer {

/**
 * @brief Mathematical ray represented by an origin and a direction.
 */
class Ray {
public:
    /**
     * @brief Build a default ray.
     */
    constexpr Ray() noexcept {}

    /**
     * @brief Build a ray from origin and direction.
     */
    constexpr Ray(const Point3D& origin, const Vector3D& direction) noexcept
        : _orig(origin), _dir(direction) {}

    /**
     * @brief Return the ray origin.
     */
    [[nodiscard]] constexpr Point3D origin() const noexcept { return _orig; }

    /**
     * @brief Return the ray direction.
     */
    [[nodiscard]] constexpr Vector3D direction() const noexcept { return _dir; }

    /**
     * @brief Evaluate point along the ray for parameter `t`.
     */
    [[nodiscard]] constexpr Point3D at(double t) const noexcept { return _orig + t * _dir; }

private:
    Point3D _orig;
    Vector3D _dir;
};

} // namespace Raytracer
