#pragma once

#include "math/Color.hpp"
#include "math/HitRecord.hpp"
#include "math/Ray.hpp"

namespace Raytracer {

class IBSDF {
public:
    virtual ~IBSDF() = default;

    virtual bool
    scatter(const Ray& ray_in, const HitRecord& hit, Color& attenuation, Ray& scattered) const = 0;

    virtual Color evaluate(const Vector3D& light_dir, const Vector3D& view_dir, const HitRecord& hit) const = 0;

    virtual Color emitted(double u, double v, const Vector3D& p) const = 0;
};

} // namespace Raytracer
