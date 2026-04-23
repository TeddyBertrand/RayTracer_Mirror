#pragma once

#include <memory>

#include "math/Ray.hpp"

namespace Raytracer {

class IMaterial;

struct HitRecord {
    Point3D point;
    Vector3D normal;
    double t = 0.0;

    double u = 0.0;
    double v = 0.0;

    bool front_face = false;

    std::shared_ptr<IMaterial> material;

    inline void setFaceNormal(const Ray& r, const Vector3D& outward_normal) noexcept {
        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

} // namespace Raytracer
