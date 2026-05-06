#include "MetalBSDF.hpp"

namespace Raytracer {

bool MetalBSDF::scatter(const Ray& r_in,
                        const HitRecord& hit,
                        Color& attenuation,
                        Ray& scattered) const {

    Vector3D reflected = Vector3D::reflect(r_in.direction().normalized(), hit.normal);

    Vector3D scatter_direction = reflected + (_fuzz * Vector3D::getRandomUnitVector());

    scattered = Ray(hit.point, scatter_direction.normalized(), RayType::REFLECT);

    attenuation = _albedo_texture->value(hit.u, hit.v, hit.point);

    return (scattered.direction().dot(hit.normal) > 0);
}

} // namespace Raytracer
