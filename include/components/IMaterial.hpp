#pragma once

#include "math/Color.hpp"
#include "math/HitRecord.hpp"
#include "math/Ray.hpp"

#include "components/IBSDF.hpp"
#include <memory>

namespace Raytracer {

/**
 * @brief Material interface.
 *
 * A material stores a pre-built BSDF that is created once at construction
 * and reused for all surface intersections.
 * Zero heap allocation per frame.
 */
class IMaterial {
public:
    virtual ~IMaterial() = default;

    /**
     * Get the pre-built BSDF for this material.
     * This BSDF is created once at construction and never changes.
     * NO allocation per call.
     */
    virtual const IBSDF& getBSDF() const = 0;
};

} // namespace Raytracer
