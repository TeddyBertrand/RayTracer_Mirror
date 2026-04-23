#include "Lambertian.hpp"

namespace Raytracer
{

Lambertian::Lambertian(const Color& albedo) : _albedo(albedo)
{
}

bool Lambertian::scatter(
    [[maybe_unused]] const Ray& r_in,
    const HitRecord& rec,
    Color& attenuation,
    Ray& scattered
) const
{
    auto scatter_direction = rec.normal + Vector3D::random_unit_vector();

    if (scatter_direction.isNearZero())
        scatter_direction = rec.normal;

    scattered = Ray(rec.point, scatter_direction.normalized());
    attenuation = _albedo;
    return true;
}

} // namespace Raytracer
