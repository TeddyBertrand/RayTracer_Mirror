#include "NormalMappingBSDF.hpp"

namespace Raytracer {

bool NormalMappingBSDF::scatter(const Ray& r_in, const HitRecord& hit, Color& attenuation, Ray& scattered) const {
    HitRecord perturbed_hit = hit;
    perturbed_hit.normal = get_perturbed_normal(hit);

    return _base->scatter(r_in, perturbed_hit, attenuation, scattered);
}

Color NormalMappingBSDF::evaluate(const Vector3D& light_dir, const Vector3D& view_dir, const HitRecord& hit) const {
    HitRecord perturbed_hit = hit;
    perturbed_hit.normal = get_perturbed_normal(hit);
        
    return _base->evaluate(light_dir, view_dir, perturbed_hit);
}

Vector3D NormalMappingBSDF::get_perturbed_normal(const HitRecord& hit) const {

        Color n_color = _normal_map->value(hit.u, hit.v, hit.point);
        Vector3D local_n(2.0 * n_color.r - 1.0, 2.0 * n_color.g - 1.0, 2.0 * n_color.b - 1.0);

        Vector3D up = std::fabs(hit.normal.y) < 0.999 ? Vector3D(0, 1, 0) : Vector3D(1, 0, 0);
        Vector3D tangent = up.cross(hit.normal);
        if (tangent.lengthSquared() < 1e-12) {
            tangent = Vector3D(1, 0, 0).cross(hit.normal);
        }
        tangent = (tangent - hit.normal * hit.normal.dot(tangent)).normalized();
        Vector3D bitangent = hit.normal.cross(tangent).normalized();

        Vector3D perturbed =
            (tangent * local_n.x + bitangent * local_n.y + hit.normal * local_n.z).normalized();

        return (hit.normal * (1.0 - _strength) + perturbed * _strength).normalized();
    }

} // namespace Raytracer
