#pragma once

#include "components/IMaterial.hpp"
#include "components/ITexture.hpp"
#include "materials/commun/bsdf/emissive/EmissiveBSDF.hpp"
#include "math/Color.hpp"
#include <memory>

namespace Raytracer {

class LightMaterial : public IMaterial {
public:
    LightMaterial(std::shared_ptr<ITexture> tex) : _bsdf(std::make_unique<EmissiveBSDF>(tex)) {}

    const IBSDF& getBSDF() const override { return *_bsdf; }

private:
    std::unique_ptr<IBSDF> _bsdf; // Pre-built once at construction
};

} // namespace Raytracer
