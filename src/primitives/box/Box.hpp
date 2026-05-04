#pragma once

#include <memory>

#include "components/IMaterial.hpp"
#include "math/AABB.hpp"
#include <components/IPrimitive.hpp>
#include <math/Vector3D.hpp>

namespace Raytracer {

class Box : public virtual IPrimitive {
public:
    /**
     * @brief Construct a new Sphere object with params
     *
     * @param center
     * @param radius
     * @param material
     */
    Box() : _min(-0.5, -0.5, -0.5), _max(0.5, 0.5, 0.5) {};

    /**
     * @brief Hit function
     *
     * check if a ray between an interval t0 and t1 has hit the sphere or not
     * @param r
     * @param ray_t
     * @param rec
     * @return true
     * @return false
     */
    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const;

    /**
     * @brief Get the Normal vector at a specific point on the Box surfaces
     *
     * @param p The hit point (in local space)
     * @return Vector3D The outward normal (unit vector)
     */
    Vector3D getNormalAt(const Vector3D& p) const;

    AABB getBoundingBox() const override { return AABB::infinite(); }

private:
    Vector3D _min;
    Vector3D _max;
    std::shared_ptr<IMaterial> _material;
};
} // namespace Raytracer
