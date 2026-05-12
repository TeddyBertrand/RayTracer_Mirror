#pragma once

#include "components/ITexture.hpp"
#include "materials/commun/bsdf/ABSDF.hpp"
#include <cmath>
#include <memory>

namespace Raytracer {

class NormalMappingBSDF : public ABSDF {
public:
    NormalMappingBSDF(std::shared_ptr<ABSDF> base,
                      std::shared_ptr<ITexture> map,
                      double strength = 1.0)
        : _base(base), _normal_map(map), _strength(std::max(0.0, std::min(1.0, strength))) {}

    Color evaluate(const Vector3D& light_dir,
                   const Vector3D& view_dir,
                   const HitRecord& hit) const override;

    bool scatter(const Ray& r_in,
                 const HitRecord& hit,
                 Color& attenuation,
                 Ray& scattered) const override;

private:
    std::shared_ptr<ABSDF> _base;
    std::shared_ptr<ITexture> _normal_map;
    double _strength;

    Vector3D get_perturbed_normal(const HitRecord& hit) const;
};

} // namespace Raytracer
