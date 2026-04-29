#pragma once

#include <memory>

#include "components/IBSDF.hpp"
#include "components/IMaterial.hpp"
#include "components/ITexture.hpp"

#include "materials/commun/bsdf/lambertian/LambertianBSDF.hpp"

namespace Raytracer {

class FlatMaterial : public IMaterial {
public:
    FlatMaterial(std::shared_ptr<ITexture> tex) : _albedo(tex) {}

    std::unique_ptr<IBSDF> getBSDF(const HitRecord& hit) const override;

private:
    std::shared_ptr<ITexture> _albedo;
};

} // namespace Raytracer
