#pragma once

#include "components/IMaterial.hpp"
#include "components/ITexture.hpp"
#include "materials/commun/bsdf/emissive/EmissiveBSDF.hpp"
#include "math/Color.hpp"
#include <memory>

namespace Raytracer {

class LightMaterial : public IMaterial {
public:
    LightMaterial(std::shared_ptr<ITexture> tex) : _emission_tex(tex) {}

    std::unique_ptr<IBSDF> getBSDF(const HitRecord& rec) const override;

private:
    std::shared_ptr<ITexture> _emission_tex;
};

} // namespace Raytracer
