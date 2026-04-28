#include "Transparent.hpp"
#include "math/MathUtils.hpp"

namespace Raytracer {

extern "C" const char* getName() { return "transparent"; }

bool Transparent::scatter(const Ray& r_in,
                          const HitRecord& rec,
                          Color& attenuation,
                          Ray& scattered) const {
    attenuation = _albedo;
    double ratio = rec.front_face ? (1.0 / _ref_idx) : _ref_idx;

    Vector3D unit_direction = r_in.direction().normalized();
    double cos_theta = std::min((-unit_direction).dot(rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ratio * sin_theta > 1.0;
    Vector3D direction;

    if (cannot_refract || Math::reflectance(cos_theta, ratio) > Math::randomDouble(0.0, 1.0)) {
        direction = Math::reflect(unit_direction, rec.normal);
    } else {
        direction = Math::refract(unit_direction, rec.normal, ratio);
    }

    scattered = Ray(rec.point, direction);
    return true;
}

} // namespace Raytracer
