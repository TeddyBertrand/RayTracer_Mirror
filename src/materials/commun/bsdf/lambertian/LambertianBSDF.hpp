#pragma once

#include "components/IBSDF.hpp"
#include "components/IMaterial.hpp"

namespace Raytracer {

class LambertianBSDF : public IBSDF {
public:
    LambertianBSDF(const Color& a) : _albedo(a) {}

    bool scatter(const Ray& r_in,
                 const HitRecord& hit,
                 Color& attenuation,
                 Ray& scattered) const override;

    Color evaluate(const Vector3D& light_dir,
                   const Vector3D& view_dir,
                   const HitRecord& hit) const override;

    Color emitted([[maybe_unused]] double u,
                  [[maybe_unused]] double v,
                  [[maybe_unused]] const Vector3D& p) const override {
        return Color(0, 0, 0);
    }

private:
    Color _albedo;
};

} // namespace Raytracer
