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
                  std::shared_ptr<IBSDF> custom_bsdf = nullptr);

    const IBSDF& getBSDF() const override { return *_bsdf; }

private:
    double _spec;
    std::shared_ptr<IBSDF> _bsdf;
};

} // namespace Raytracer
