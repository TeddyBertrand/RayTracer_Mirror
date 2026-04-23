#pragma once

#include "materials/commun/AMaterial.hpp"

namespace Raytracer {

class Lambertian : public AMaterial {
public:
    Lambertian(const Color& albedo);

    bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation,
                 Ray& scattered) const override;

    std::string getName() const override { return "lambertian"; }

private:
    Color _albedo;
};

} // namespace Raytracer
