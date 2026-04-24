#pragma once

#include "math/Color.hpp"
#include "math/HitRecord.hpp"
#include "math/Ray.hpp"

#include "components/IPlugin.hpp"

namespace Raytracer {

/**
 * @brief Interface for BRDF/BSDF-like material behavior.
 *
 * Materials define how incoming rays interact with surfaces: they can scatter
 * rays, attenuate light, transmit energy and emit radiance.
 */
class IMaterial : public IPlugin {
public:
    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     */
    virtual ~IMaterial() = default;

    /**
     * @brief Compute scattered ray and attenuation for an incoming ray hit.
     *
     * @param r_in Incoming ray.
     * @param rec Intersection information.
     * @param attenuation Output multiplicative color attenuation.
     * @param scattered Output scattered ray.
     * @return true if scattering occurs, false if the ray is absorbed.
     */
    virtual bool
    scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;

    /**
     * @brief Return the specular blend factor used by the shading model.
     */
    virtual double getSpecularWeight() const = 0;

    /**
     * @brief Return per-channel transmittance of the material.
     */
    virtual Color getTransmittance() const = 0;

    /**
     * @brief Return emitted radiance at the hit point.
     *
     * Non-emissive materials typically return black.
     */
    virtual Color emit(const HitRecord& rec) const = 0;
};

} // namespace Raytracer
