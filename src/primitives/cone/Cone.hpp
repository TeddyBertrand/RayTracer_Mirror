#pragma once

#include <memory>

#include "components/IMaterial.hpp"
#include <components/IPrimitive.hpp>
#include <math/Vector3D.hpp>

namespace Raytracer {

class Cone : public virtual IPrimitive {
public:
    /**
     * @brief Construct a new Cone object
     */
    Cone() : _material(nullptr) {}

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

    void setMaterial(std::shared_ptr<IMaterial> m) override { _material = m; }

private:
    std::shared_ptr<IMaterial> _material;
    bool solve(const Ray& r, float& t0, float& t1) const;
};
} // namespace Raytracer
