#pragma once

#include "components/IBSDF.hpp"

namespace Raytracer {

/**
 * @brief Emissive BSDF for light sources.
 *
 * - `emitted()` returns the light color.
 * - `evaluate()` is black.
 * - `scatter()` returns false because the surface does not bounce light.
 */
class EmissiveBSDF : public IBSDF {
public:
    explicit EmissiveBSDF(const Color& c) : _emitColor(c) {}

    /**
     * No BRDF contribution for emitters.
     */
    Color evaluate([[maybe_unused]] const Vector3D& wo,
                   [[maybe_unused]] const Vector3D& wi,
                   [[maybe_unused]] const HitRecord& rec) const override {
        return Color(0, 0, 0);
    }

    /**
     * Emitters do not scatter incoming rays.
     */
    bool scatter([[maybe_unused]] const Ray& ray,
                 [[maybe_unused]] const HitRecord& rec,
                 [[maybe_unused]] Color& color,
                 [[maybe_unused]] Ray& scattered) const override {
        return false;
    }

    /**
     * Return the constant emitted radiance.
     */
    Color emitted([[maybe_unused]] double u,
                  [[maybe_unused]] double v,
                  [[maybe_unused]] const Point3D& p) const override {
        return _emitColor;
    }

private:
    Color _emitColor;
};

} // namespace Raytracer
