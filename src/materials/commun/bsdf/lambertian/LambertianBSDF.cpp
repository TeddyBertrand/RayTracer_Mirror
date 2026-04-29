#include "LambertianBSDF.hpp"

namespace Raytracer {

bool LambertianBSDF::scatter([[maybe_unused]] const Ray& r_in,
                             const HitRecord& hit,
                             Color& attenuation,
                             Ray& scattered) const {
    auto scatter_direction = hit.normal + Vector3D::getRandomUnitVector();

    if (scatter_direction.isNearZero())
        scatter_direction = hit.normal;

    scattered = Ray(hit.point, scatter_direction, RayType::DIFFUSE);
    attenuation = _albedo;
    return true;
}

Color LambertianBSDF::evaluate(const Vector3D& light_dir,
                               [[maybe_unused]] const Vector3D& view_dir,
                               const HitRecord& hit) const {
    double cos_theta = std::max(0.0, hit.normal.dot(light_dir));

    return (_albedo / 3.1415926535) * cos_theta;
}

} // namespace Raytracer
