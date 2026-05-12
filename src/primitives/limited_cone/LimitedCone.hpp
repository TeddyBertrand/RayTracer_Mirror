#pragma once

#include "components/IPrimitive.hpp"
#include "math/AABB.hpp"
#include "math/HitRecord.hpp"
#include "math/Interval.hpp"
#include "math/Vector3D.hpp"

namespace Raytracer {

class LimitedCone : public virtual IPrimitive {
public:
    /**
     * @brief Construct a new Limited Cone object.
     *
     * @param height The vertical extent of the cone along the Y-axis. Default is 1.0.
     * @param radius The radius of the circular base at Y = 0. Default is 1.0.
     *
     * @note The internal constant _k is pre-calculated as (radius/height)² to optimize
     * intersection calculations.
     */
    LimitedCone(float height = 1.0f, float radius = 1.0f) : _height(height), _radius(radius) {
        _k = (_radius / _height) * (_radius / _height);
    }

    /**
     * @brief Default destructor.
     */
    ~LimitedCone() override = default;

    /**
     * @brief Checks for a ray intersection within a specific distance interval.
     *
     * @param r The ray being cast.
     * @param ray_t The valid range (t_min to t_max) for the intersection distance.
     * @param rec A HitRecord structure to store intersection data (point, normal, UV, etc.).
     * @return true If the ray hits either the body or the base of the cone.
     * @return false If no intersection is found within the given interval.
     */
    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;

    /**
     * @brief Computes the Axis-Aligned Bounding Box (AABB) for the cone.
     *
     * @details The box is computed in local space, centered on the Y-axis,
     * spanning from -radius to +radius on X/Z and 0 to height on Y.
     *
     * @return AABB The bounding volume of the primitive.
     */
    AABB getBoundingBox() const override {
        return AABB(Vector3D(-_radius, 0, -_radius), Vector3D(_radius, _height, _radius));
    }

private:
    /**
     * @brief Calculates intersection with the lateral surface of the cone.
     *
     * @param r The ray to test.
     * @param ray_t The valid interval for the intersection.
     * @param[out] t_hit Stores the distance 't' if an intersection occurs.
     * @return true If the ray intersects the side of the cone.
     */
    bool intersectBody(const Ray& r, Interval ray_t, float& t_hit) const;

    /**
     * @brief Calculates intersection with the bottom circular base.
     *
     * @param r The ray to test.
     * @param ray_t The valid interval for the intersection.
     * @param[out] t_hit Stores the distance 't' if an intersection occurs.
     * @return true If the ray intersects the base disc at Y = 0.
     */
    bool intersectBase(const Ray& r, Interval ray_t, float& t_hit) const;

    float _height; /**< Vertical height of the cone. */
    float _radius; /**< Maximum radius at the base. */
    float _k;      /**< Geometric constant (radius/height)² for the quadratic equation. */
};

} // namespace Raytracer