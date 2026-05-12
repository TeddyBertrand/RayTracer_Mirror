#pragma once

#include <memory>

#include "components/IBSDF.hpp"
#include "components/IMaterial.hpp"
#include "components/ITexture.hpp"

#include "materials/commun/bsdf/phong/PhongBSDF.hpp"

namespace Raytracer {

class PhongMaterial : public IMaterial {
public:
    PhongMaterial(std::shared_ptr<ITexture> tex,
                  double spec = 0.0,
                  std::shared_ptr<IBSDF> custom_bsdf = nullptr)
        : _spec(spec < 0.0 ? 0.0 : (spec > 1.0 ? 1.0 : spec)) {
        if (custom_bsdf) {
            _bsdf = custom_bsdf;
        } else {
            _bsdf = std::make_shared<PhongBSDF>(tex, _spec);
        }
    }

    const IBSDF& getBSDF() const override { return *_bsdf; }

private:
    double _spec;
    std::shared_ptr<IBSDF> _bsdf;
};

} // namespace Raytracer
