#pragma once

#include <memory>

#include "components/IMaterial.hpp"
#include <components/IPrimitive.hpp>
#include <math/Vector3D.hpp>

namespace Raytracer {

class Cylinder : public virtual IPrimitive {
public:
    /**
     * @brief Construct a new Cylinder object with params
     *
     * @param center Center of the bottom base of the cylinder
     * @param radius Radius of the cylinder
     * @param height Height of the cylinder (along Y axis)
     * @param material Material applied to the cylinder
     */
    Cylinder() : _center({0, 0, 0}), _radius(1), _height(1) {}

    /**
     * @brief Hit function
     *
     * Check if a ray between an interval t0 and t1 has hit the cylinder or not
     * @param r Ray to test
     * @param ray_t Interval for valid hit distance
     * @param rec Output hit information
     * @return true if a valid hit exists in the interval
     * @return false otherwise
     */
    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const;

    AABB getBoundingBox() const override {
        Point3D radiusVec(_radius, 0.0, _radius);
        Point3D heightVec(0.0, _height, 0.0);
        return AABB{_center - radiusVec, _center + radiusVec + heightVec};
    }

private:
    Point3D _center; // Center of the bottom base
    double _radius;  // Radius of the cylinder
    double _height;  // Height along Y axis
};

}; // namespace Raytracer
