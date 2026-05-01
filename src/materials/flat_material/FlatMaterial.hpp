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
        : _randomness(randomness < 0.0 ? 0.0 : (randomness > 1.0 ? 1.0 : randomness)),
          _bsdf(std::make_unique<LambertianBSDF>(tex, _randomness)) {}

    const IBSDF& getBSDF() const override { return *_bsdf; }

private:
    double _randomness;
    std::unique_ptr<IBSDF> _bsdf; // Pre-built once at construction
};

} // namespace Raytracer
