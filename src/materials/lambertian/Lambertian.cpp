#include "Lambertian.hpp"

namespace Raytracer {

extern "C" const char* getName() { return "lambertian"; }

Lambertian::Lambertian(const Color& albedo) : _albedo(albedo) {}

bool Lambertian::scatter([[maybe_unused]] const Ray& r_in,
                         const HitRecord& rec,
                         Color& attenuation,
                         Ray& scattered) const {
    auto scatter_direction = rec.normal + Vector3D::getRandomUnitVector();

    if (scatter_direction.isNearZero())
        scatter_direction = rec.normal;

    scattered = Ray(rec.point, scatter_direction.normalized());
    attenuation = _albedo;
    return true;
}

} // namespace Raytracer
