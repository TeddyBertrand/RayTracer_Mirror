#pragma once

#include <memory>

#include "components/IBSDF.hpp"
#include "components/IMaterial.hpp"
#include "components/ITexture.hpp"

#include "materials/commun/bsdf/lambertian/LambertianBSDF.hpp"

namespace Raytracer {

class FlatMaterial : public IMaterial {
public:
    FlatMaterial(std::shared_ptr<ITexture> tex, double randomness = 1.0)
        : _albedo(tex),
          _randomness(randomness < 0.0 ? 0.0 : (randomness > 1.0 ? 1.0 : randomness)) {}

    std::unique_ptr<IBSDF> getBSDF(const HitRecord& hit) const override;

private:
    std::shared_ptr<ITexture> _albedo;
    double _randomness;
};

} // namespace Raytracer
