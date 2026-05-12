#pragma once

#include <memory>

#include "components/IBSDF.hpp"
#include "components/IMaterial.hpp"
#include "components/ITexture.hpp"

#include "materials/commun/bsdf/metal/MetalBSDF.hpp"

namespace Raytracer {

class MetalMaterial : public IMaterial {
public:
    MetalMaterial(std::shared_ptr<ITexture> tex,
                  double fuzz = 0.0,
                  std::shared_ptr<IBSDF> custom_bsdf = nullptr)
        : _fuzz(fuzz < 0.0 ? 0.0 : (fuzz > 1.0 ? 1.0 : fuzz)) {
        if (custom_bsdf) {
            _bsdf = custom_bsdf;
        } else {
            _bsdf = std::make_shared<MetalBSDF>(tex, _fuzz);
        }
    }

    const IBSDF& getBSDF() const override { return *_bsdf; }

private:
    double _fuzz;
    std::shared_ptr<IBSDF> _bsdf; // Pre-built once at construction
};

} // namespace Raytracer
