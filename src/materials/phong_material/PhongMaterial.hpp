#pragma once

#include <memory>

#include "components/IBSDF.hpp"
#include "components/IMaterial.hpp"
#include "components/ITexture.hpp"

#include "materials/commun/bsdf/phong/PhongBSDF.hpp"

namespace Raytracer {

class PhongMaterial : public IMaterial {
public:
    PhongMaterial(std::shared_ptr<ITexture> tex, double spec = 0.0)
        : _spec(spec < 0.0 ? 0.0 : (spec > 1.0 ? 1.0 : spec)),
          _bsdf(std::make_unique<PhongBSDF>(tex, _spec)) {}

    const IBSDF& getBSDF() const override { return *_bsdf; }

private:
    double _spec;
    std::unique_ptr<IBSDF> _bsdf; // Pre-built once at construction
};

} // namespace Raytracer
