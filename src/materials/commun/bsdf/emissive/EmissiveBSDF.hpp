#pragma once

#include "components/ITexture.hpp"
#include "materials/commun/bsdf/ABSDF.hpp"
#include <memory>

namespace Raytracer {

/**
 * @brief Emissive BSDF for light sources.
 *
 * - `emitted()` evaluates the emission texture at u,v.
 * - `evaluate()` is black.
 * - `scatter()` returns false because the surface does not bounce light.
 */
class EmissiveBSDF : public ABSDF {
public:
    explicit EmissiveBSDF(std::shared_ptr<ITexture> tex) : _emission_texture(tex) {}

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
     * Return the emission color evaluated from the texture at u,v.
     */
    Color emitted([[maybe_unused]] double u,
                  [[maybe_unused]] double v,
                  [[maybe_unused]] const Point3D& p) const override {
        return _emission_texture->value(u, v);
    }

private:
    std::shared_ptr<ITexture> _emission_texture;
};

} // namespace Raytracer
