#pragma once

#include "math/Vector3D.hpp"

namespace Raytracer {

/**
 * @brief Enum defining the purpose of a ray.
 * Used by materials and shaders to decide how to respond (e.g., Sky reflection vs Sky lighting).
 */
enum class RayType {
    CAMERA,    /**< Primary ray from the camera. */
    REFLECT,   /**< Specular ray (sharp reflections/refractions). */
    DIFFUSE,   /**< Indirect ray (diffuse bounce for global illumination). */
    SHADOW     /**< Ray used for visibility testing towards light sources. */
};

/**
 * @brief Mathematical ray represented by an origin, a direction and a type.
 */
class Ray {
public:
    /**
     * @brief Build a default ray (origin at 0,0,0, direction forward, type CAMERA).
     */
    constexpr Ray() noexcept 
        : _orig(0, 0, 0), _dir(0, 0, 1), _type(RayType::CAMERA) {}

    /**
     * @brief Build a ray from origin, direction and type.
     * @param origin The starting point of the ray.
     * @param direction The direction vector of the ray.
     * @param type The nature of the ray (default is CAMERA).
     */
    constexpr Ray(const Point3D& origin, const Vector3D& direction, RayType type = RayType::CAMERA) noexcept
        : _orig(origin), _dir(direction), _type(type) {}

    /**
     * @brief Return the ray origin.
     */
    [[nodiscard]] constexpr Point3D origin() const noexcept { return _orig; }

    /**
     * @brief Return the ray direction.
     */
    [[nodiscard]] constexpr Vector3D direction() const noexcept { return _dir; }

    /**
     * @brief Return the ray type.
     */
    [[nodiscard]] constexpr RayType type() const noexcept { return _type; }

    /**
     * @brief Evaluate point along the ray for parameter `t`.
     */
    [[nodiscard]] constexpr Point3D at(double t) const noexcept { return _orig + t * _dir; }

private:
    Point3D _orig;
    Vector3D _dir;
    RayType _type;
};

} // namespace Raytracer
