#pragma once

#include "materials/commun/bsdf/ABSDF.hpp"

namespace Raytracer {

/**
 * @brief Diffuse cosine-weighted BSDF.
 *
 * Lambertian = energy-conserving diffuse reflection.
 * - `evaluate()` returns `albedo / pi`.
 * - `scatter()` samples a cosine-weighted hemisphere.
 * - `emitted()` always returns black.
 */
class LambertianBSDF : public ABSDF {
public:
    explicit LambertianBSDF(const Color& a) : _albedo(a) {}

    /**
     * Sample one diffuse bounce.
     *
     * `attenuation` is the albedo; `scattered` starts at the hit point.
     */
    bool scatter(const Ray& r_in,
                 const HitRecord& hit,
                 Color& attenuation,
                 Ray& scattered) const override;

    /**
     * Return the diffuse BRDF value: `_albedo / pi`.
     */
    Color evaluate(const Vector3D& light_dir,
                   const Vector3D& view_dir,
                   const HitRecord& hit) const override;

private:
    Color _albedo;
};

} // namespace Raytracer
