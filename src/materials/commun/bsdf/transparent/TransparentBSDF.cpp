#include "TransparentBSDF.hpp"
#include "math/MathUtils.hpp"

namespace Raytracer {

bool TransparentBSDF::scatter(const Ray& r_in,
                        const HitRecord& hit,
                        Color& attenuation,
                        Ray& scattered) const {
    
    attenuation = _albedo_texture->value(hit.u, hit.v);

    double ratio = hit.front_face ? (1.0 / _ref) : _ref;

    Vector3D unit_direction = r_in.direction().normalized();
    double cos_theta = std::min((-unit_direction).dot(hit.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ratio * sin_theta > 1.0;
    Vector3D direction;

    if (cannot_refract || Math::reflectance(cos_theta, ratio) > Math::randomDouble(0.0, 1.0)) {
        direction = Math::reflect(unit_direction, hit.normal);
    } else {
        direction = Math::refract(unit_direction, hit.normal, ratio);
    }

    scattered = Ray(hit.point, direction);
    return true;
}

Color TransparentBSDF::evaluate(const Vector3D& light_dir,
                               [[maybe_unused]] const Vector3D& view_dir,
                               const HitRecord& hit) const {
    double cos_theta = std::max(0.0, hit.normal.dot(light_dir));
    Color albedo = _albedo_texture->value(hit.u, hit.v);
    return (albedo / M_PI) * cos_theta;
}

}
