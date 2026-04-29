#pragma once

#include "materials/commun/AMaterial.hpp"
#include "parser/ISettings.hpp"
namespace Raytracer {

class Lambertian : public AMaterial {
public:
    Lambertian(const Color& albedo);

    bool scatter(const Ray& r_in,
                 const HitRecord& rec,
                 Color& attenuation,
                 Ray& scattered) const override;

private:
    Color _albedo;
};

} // namespace Raytracer
