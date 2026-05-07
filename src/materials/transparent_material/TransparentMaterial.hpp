#pragma once

#include <memory>

#include "components/IBSDF.hpp"
#include "components/IMaterial.hpp"
#include "components/ITexture.hpp"

#include "materials/commun/bsdf/transparent/TransparentBSDF.hpp"

namespace Raytracer {

class TransparentMaterial : public IMaterial {
public:
    TransparentMaterial(std::shared_ptr<ITexture> tex, double fuzz = 0.0)
        : _fuzz(fuzz < 0.0 ? 0.0 : (fuzz > 1.0 ? 1.0 : fuzz)),
          _bsdf(std::make_unique<TransparentBSDF>(tex, _fuzz)) {}

    const IBSDF& getBSDF() const override { return *_bsdf; }

private:
    double _fuzz;
    std::unique_ptr<IBSDF> _bsdf; // Pre-built once at construction
};

} // namespace Raytracer
