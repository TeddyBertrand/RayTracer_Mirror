#pragma once

#include "components/ITexture.hpp"
#include "materials/commun/bsdf/ABSDF.hpp"
#include <memory>

namespace Raytracer {

/**
 * @brief Diffuse cosine-weighted BSDF.
 *
 * Lambertian = energy-conserving diffuse reflection.
 * - `evaluate()` returns `texture->value(u,v) / pi`.
 * - `scatter()` samples a cosine-weighted hemisphere.
 * - `emitted()` always returns black.
 * - Texture is evaluated per-hit from HitRecord UVs.
 */
class LambertianBSDF : public ABSDF {
public:
    explicit LambertianBSDF(std::shared_ptr<ITexture> tex, double randomness = 1.0)
        : _albedo_texture(tex),
          _randomness(randomness < 0.0 ? 0.0 : (randomness > 1.0 ? 1.0 : randomness)) {}

    /**
     * Sample one diffuse bounce.
     *
     * `attenuation` is the texture-evaluated albedo; `scattered` starts at the hit point.
     */
    bool scatter(const Ray& r_in,
                 const HitRecord& hit,
                 Color& attenuation,
                 Ray& scattered) const override;

    /**
     * Return the diffuse BRDF value: `texture->value(u,v) / pi`.
     */
    Color evaluate(const Vector3D& light_dir,
                   const Vector3D& view_dir,
                   const HitRecord& hit) const override;

private:
    std::shared_ptr<ITexture> _albedo_texture;
    double _randomness;
};

} // namespace Raytracer
