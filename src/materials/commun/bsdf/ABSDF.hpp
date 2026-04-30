#pragma once

#include "components/IBSDF.hpp"

namespace Raytracer {

/**
 * @brief Common abstract base for BSDF implementations.
 *
 * This class exists to share the default non-emissive behavior and to give all
 * concrete BSDFs a single inheritance point inside the `bsdf` folder.
 *
 * Typical contract for derived classes:
 * - implement `scatter()` when the material can bounce rays;
 * - implement `evaluate()` for the BRDF/BSDF value;
 * - override `emitted()` only for emissive BSDFs.
 */
class ABSDF : public IBSDF {
public:
    ~ABSDF() override = default;

    /**
     * Default BSDF emission is black.
     */
    Color emitted([[maybe_unused]] double u,
                  [[maybe_unused]] double v,
                  [[maybe_unused]] const Vector3D& p) const override {
        return Color(0, 0, 0);
    }
};

} // namespace Raytracer