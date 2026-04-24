#pragma once

#include <memory>

#include "math/Ray.hpp"

namespace Raytracer {

class IMaterial;

/**
 * @brief Stores detailed information about a ray/primitive intersection.
 */
struct HitRecord {
    /** World-space hit position. */
    Point3D point;
    /** Surface normal oriented against the incident ray. */
    Vector3D normal;
    /** Distance parameter along the ray (`ray.at(t)`). */
    double t = 0.0;

    /** Horizontal texture coordinate. */
    double u = 0.0;
    /** Vertical texture coordinate. */
    double v = 0.0;

    /** True when the ray hits the outward side of the surface. */
    bool front_face = false;

    /** Material bound to the intersected primitive. */
    std::shared_ptr<IMaterial> material;

    /**
     * @brief Compute and orient the normal consistently with the incoming ray.
     *
     * @param r Incident ray.
     * @param outward_normal Geometric outward normal from the primitive.
     */
    inline void setFaceNormal(const Ray& r, const Vector3D& outward_normal) noexcept {
        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

} // namespace Raytracer
