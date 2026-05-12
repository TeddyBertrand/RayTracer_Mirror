#pragma once

#include <memory>

#include "components/IBSDF.hpp"
#include "components/IMaterial.hpp"
#include "components/ITexture.hpp"

#include "materials/commun/bsdf/lambertian/LambertianBSDF.hpp"

namespace Raytracer {

class FlatMaterial : public IMaterial {
public:
    FlatMaterial(std::shared_ptr<ITexture> tex,
                 double randomness = 1.0,
                 std::shared_ptr<IBSDF> custom_bsdf = nullptr);

    const IBSDF& getBSDF() const override { return *_bsdf; }

private:
    double _randomness;
    std::shared_ptr<IBSDF> _bsdf;
};

} // namespace Raytracer
