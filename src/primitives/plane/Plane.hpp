#pragma once

#include <memory>

#include "components/IMaterial.hpp"
#include "math/AABB.hpp"
#include <components/IPrimitive.hpp>
#include <math/Vector3D.hpp>

namespace Raytracer {

class Plane : public virtual IPrimitive {
public:
    /**
     * @brief Construct a new Sphere object with params
     *
     * @param center
     * @param radius
     * @param material
     */
    Plane() : _position(0, 0, 0), _normal(0, 1, 0) {}

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

    AABB getBoundingBox() const override { return AABB::infinite(); }

private:
    Vector3D _position;
    Vector3D _normal;
    double _radius;
};
} // namespace Raytracer
