#pragma once

#include "math/Color.hpp"
#include "math/HitRecord.hpp"
#include "math/Ray.hpp"

#include "components/IBSDF.hpp"

namespace Raytracer {

/**
 * @brief Interface for BRDF/BSDF-like material behavior.
 *
 * Materials define how incoming rays interact with surfaces: they can scatter
 * rays, attenuate light, transmit energy and emit radiance.
 */
class IMaterial {
public:
    virtual ~IMaterial() = default;
    // Le matériau crée un BSDF à la volée lors de l'impact
    virtual std::unique_ptr<IBSDF> getBSDF(const HitRecord& hit) const = 0;
};

} // namespace Raytracer
