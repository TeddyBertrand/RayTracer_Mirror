#pragma once

#include <memory>

#include "components/IMaterial.hpp"
#include <components/IPrimitive.hpp>
#include <math/Vector3D.hpp>

namespace Raytracer {

extern "C" std::string getName() { return "sphere"; }

class Sphere : public virtual IPrimitive {
public:
    /**
     * @brief Construct a new Sphere object with params
     *
     * @param center
     * @param radius
     * @param material
     */
    Sphere(Point3D center, double radius, std::shared_ptr<IMaterial> material)
        : _center(center), _radius(radius), _material(material) {}

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

private:
    Point3D _center;
    double _radius;
    std::shared_ptr<IMaterial> _material;
};
}; // namespace Raytracer
