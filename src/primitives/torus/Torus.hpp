#pragma once

#include <memory>

#include "components/IMaterial.hpp"
#include "math/AABB.hpp"
#include "math/MathUtils.hpp"
#include <components/IPrimitive.hpp>
#include <math/Vector3D.hpp>

namespace Raytracer {

class Torus : public virtual IPrimitive {
public:
    /**
     * @brief Construct a new Torus object with params
     *
     * @param center
     * @param radius
     * @param material
     */
    Torus(double majorR, double minorR) : _majorRadius(majorR), _minorRadius(minorR) {}

    /**
     * @brief Hit function
     *
     * check if a ray between an interval t0 and t1 has hit the Torus or not
     * @param r
     * @param ray_t
     * @param rec
     * @return true
     * @return false
     */
    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const;

    AABB getBoundingBox() const {
        double r_total = _majorRadius + _minorRadius;
        return AABB(Vector3D(-r_total, -_minorRadius, -r_total),
                    Vector3D(r_total, _minorRadius, r_total));
    }

private:
    Math::QuarticCoeffs computeCoefficients(const Ray& r) const;
    bool
    findClosestValidT(const double roots[4], int numRoots, Interval ray_t, double& t_out) const;
    void fillHitRecord(const Ray& r, double t, HitRecord& rec) const;

    double _majorRadius;
    double _minorRadius;
};
} // namespace Raytracer
