#pragma once

#include "math/Color.hpp"
#include "math/HitRecord.hpp"
#include "math/Ray.hpp"

namespace Raytracer {

/**
 * @brief Per-hit BSDF interface.
 *
 * This is the shading model used by the renderer at one intersection.
 *
 * Main idea:
 * - `evaluate()` returns the BRDF/BSDF value for a light/view direction pair.
 * - `scatter()` samples a new ray and writes its attenuation.
 * - `emitted()` returns self-emission for lights, black otherwise.
 */
class IBSDF {
public:
    virtual ~IBSDF() = default;

    /**## FIXES

Issue #

## Description

### How

### Testing
1. **Execution**:
2. **Validation**: 

### Notes
- **Next**:
- **Technical Details**: 
- **Refactoring**: 

     * Sample a scattered ray.
     *
     * `attenuation` is the color multiplier for the bounce. Return `false` when
     * the BSDF does not scatter (for example, pure emitters).
     */
    virtual bool
    scatter(const Ray& ray_in, const HitRecord& hit, Color& attenuation, Ray& scattered) const = 0;

    /**
     * Evaluate f(light_dir, view_dir) at this hit.
     *
     * Directions should be unit vectors in world space.
     */
    virtual Color
    evaluate(const Vector3D& light_dir, const Vector3D& view_dir, const HitRecord& hit) const = 0;

    /**
     * Return emitted radiance.
     *
     * Use black for non-emissive surfaces.
     */
    virtual Color emitted(double u, double v, const Vector3D& p) const = 0;
};

} // namespace Raytracer
