#include "PhongBSDF.hpp"
#include "math/MathUtils.hpp"
#include "math/Vector3D.hpp"

#include <algorithm>
#include <cmath>

namespace Raytracer {

bool PhongBSDF::scatter(const Ray& r_in,
                        const HitRecord& hit,
                        Color& attenuation,
                        Ray& scattered) const {
    const double spec_probability = std::clamp(_spec, 0.0, 1.0);

    if (Math::randomDouble(0.0, 1.0) < spec_probability) {
        const Vector3D reflected = Math::reflect(r_in.direction().normalized(), hit.normal);

        const double roughness = 0.03 * (1.0 - spec_probability);
        Vector3D dir = (reflected + Vector3D::randomInUnitSphere() * roughness).normalized();

        scattered = Ray(hit.point + hit.normal * 0.001, dir, RayType::REFLECT);
        attenuation = Color(1.0, 1.0, 1.0);
    } else {
        Vector3D dir = hit.normal + Vector3D::getRandomUnitVector();
        if (dir.isNearZero())
            dir = hit.normal;

        dir = dir.normalized();
        scattered = Ray(hit.point + hit.normal * 0.001, dir, RayType::DIFFUSE);
        attenuation = _albedo_texture->value(hit.u, hit.v, hit.point);
    }
    return true;
}

Color PhongBSDF::evaluate(const Vector3D& light_dir,
                          const Vector3D& view_dir,
                          const HitRecord& hit) const {
    const Vector3D L = light_dir.normalized();
    const Vector3D V = view_dir.normalized();
    const double n_dot_l = std::max(0.0, hit.normal.dot(L));
    if (n_dot_l <= 0.0)
        return Color(0.0, 0.0, 0.0);

    const Color albedo = _albedo_texture->value(hit.u, hit.v, hit.point);
    const Color diffuse = (albedo / M_PI) * n_dot_l;

    const Vector3D R = Math::reflect(-L, hit.normal).normalized();
    const double spec_angle = std::max(0.0, R.dot(V));
    const double shininess = std::max(1.0, 8.0 + _spec * 248.0);
    const double spec_strength = std::clamp(_spec, 0.0, 1.0);
    const double specular_factor = std::pow(spec_angle, shininess) * spec_strength;

    const Color specular = Color(1.0, 1.0, 1.0) * specular_factor;

    return diffuse + specular;
}

} // namespace Raytracer
