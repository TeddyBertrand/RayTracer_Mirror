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

} // namespace Raytracer
