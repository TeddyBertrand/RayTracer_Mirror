#pragma once

#include <memory>

#include "components/IMaterial.hpp"
#include <components/IPrimitive.hpp>
#include <math/Vector3D.hpp>

namespace Raytracer {

class Sphere : public virtual IPrimitive {
public:
    /**
     * @brief Construct a new Sphere object with params
     *
     * @param center
     * @param radius
     * @param material
     */

    Sphere() : _center(0.0, 0.0, 0.0), _radius(1.0) {}

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

    AABB getBoundingBox() const override {
        Point3D radiusVec(_radius, _radius, _radius);
        return AABB{_center - radiusVec, _center + radiusVec};
    }

    void setMaterial(std::shared_ptr<IMaterial> m) override { _material = m; }

private:
    Point3D _center;
    double _radius;
    std::shared_ptr<IMaterial> _material;
};
}; // namespace Raytracer
