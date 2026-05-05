#pragma once

#include <memory>

#include "components/IPrimitive.hpp"
#include "math/Vector3D.hpp"

namespace Raytracer {

class InfiniteCylinder : public virtual IPrimitive {
public:
    /**
     * Unit infinite cylinder aligned on Y axis. Radius = 1 in local space.
     * Use Entity scale to change radius; Y scale is ignored to keep it infinite.
     */
    InfiniteCylinder() : _center(0.0, 0.0, 0.0), _radius(1.0) {}

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;

    AABB getBoundingBox() const override { return AABB::infinite(); }

private:
    Point3D _center; // center line goes through xz at _center.xz
    double _radius;
};

} // namespace Raytracer
