#pragma once

#include <memory>

#include "components/IBSDF.hpp"
#include "components/IMaterial.hpp"
#include "components/ITexture.hpp"

#include "materials/commun/bsdf/transparent/TransparentBSDF.hpp"

namespace Raytracer {

class TransparentMaterial : public IMaterial {
public:
    TransparentMaterial(std::shared_ptr<ITexture> tex,
                        double ref = 0.0,
                        std::shared_ptr<IBSDF> custom_bsdf = nullptr)
        : _ref(ref < 0.0 ? 0.0 : ref) {
        if (custom_bsdf) {
            _bsdf = custom_bsdf;
        } else {
            _bsdf = std::make_shared<TransparentBSDF>(tex, _ref);
        }
    }

    const IBSDF& getBSDF() const override { return *_bsdf; }

private:
    double _ref;
    std::shared_ptr<IBSDF> _bsdf; // Pre-built once at construction
};

} // namespace Raytracer
