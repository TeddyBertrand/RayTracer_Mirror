#include "LambertianBSDF.hpp"

namespace Raytracer {

bool LambertianBSDF::scatter([[maybe_unused]] const Ray& r_in,
                             const HitRecord& hit,
                             Color& attenuation,
                             Ray& scattered) const {
    if (_randomness <= 0.0) {
        attenuation = _albedo_texture->value(hit.u, hit.v);
        return false;
    }

    Vector3D scatter_direction;
    auto random_direction = Vector3D::getRandomUnitVector();
    scatter_direction =
        ((1.0 - _randomness) * hit.normal) + (_randomness * (hit.normal + random_direction));

    if (scatter_direction.isNearZero())
        scatter_direction = hit.normal;

    scattered = Ray(hit.point, scatter_direction.normalized(), RayType::DIFFUSE);
    attenuation = _albedo_texture->value(hit.u, hit.v);
    return true;
}

Color LambertianBSDF::evaluate(const Vector3D& light_dir,
                               [[maybe_unused]] const Vector3D& view_dir,
                               const HitRecord& hit) const {
    double cos_theta = std::max(0.0, hit.normal.dot(light_dir));
    Color albedo = _albedo_texture->value(hit.u, hit.v);
    return (albedo / M_PI) * cos_theta;
}

} // namespace Raytracer
