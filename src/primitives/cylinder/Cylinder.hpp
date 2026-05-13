#pragma once

#include <memory>

#include "components/IMaterial.hpp"
#include <components/IPrimitive.hpp>
#include <math/Vector3D.hpp>

namespace Raytracer {

class Cylinder : public virtual IPrimitive {
public:
    /**
     * @brief Construct a unit cylinder aligned on the Y axis.
     *
     * Base center is at the origin, radius = 1, height = 1.
     * Use entity transforms to position/scale the cylinder in the scene.
     */
    Cylinder() : _center(0.0, 0.0, 0.0), _radius(1.0), _height(1.0) {}

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

    void dump(int indent) const override {
        std::cout << std::string(indent, ' ') << "- \033[1;33m[Cylinder]\033[0m" << std::endl;
    }

private:
    Point3D _center; // Center of the bottom base in local space
    double _radius;  // Local radius
    double _height;  // Local height along Y axis
};

}; // namespace Raytracer
