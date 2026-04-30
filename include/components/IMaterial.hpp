#pragma once

#include "math/Color.hpp"
#include "math/HitRecord.hpp"
#include "math/Ray.hpp"

#include "components/IBSDF.hpp"

namespace Raytracer {

/**
 * @brief Material interface.
 *
 * A material is a factory for per-hit shading behavior. It stores shared
 * parameters, then creates an `IBSDF` for each surface intersection.
 *
 * Contract:
 * - `getBSDF()` must return the shading model for the given `HitRecord`.
 * - Returned BSDFs must implement scattering, evaluation, and emission.
 * - Colors are linear RGB; keep implementations energy-aware.
 */
class IMaterial {
public:
    virtual ~IMaterial() = default;
    /**
     * Create the BSDF used to shade one hit.
     *
     * `hit` contains position, normal, and UVs. Return `nullptr` only when the
     * material cannot shade this intersection.
     */
    virtual std::unique_ptr<IBSDF> getBSDF(const HitRecord& hit) const = 0;
};

} // namespace Raytracer
