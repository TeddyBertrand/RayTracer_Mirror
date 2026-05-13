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
    Cone() = default;

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

    void dump(int indent) const override {
        std::cout << std::string(indent, ' ') << "- \033[1;33m[Cone]\033[0m" << std::endl;
    }

private:
    bool solve(const Ray& r, float& t0, float& t1) const;
};
} // namespace Raytracer
