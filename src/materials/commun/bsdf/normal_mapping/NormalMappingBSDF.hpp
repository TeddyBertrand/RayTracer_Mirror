#pragma once

#include "components/ITexture.hpp"
#include "materials/commun/bsdf/ABSDF.hpp"
#include <cmath>
#include <memory>

namespace Raytracer {

class NormalMappingBSDF : public ABSDF {
public:
    NormalMappingBSDF(std::shared_ptr<ABSDF> base, std::shared_ptr<ITexture> map)
        : _base(base), _normal_map(map) {}

    Color evaluate(const Vector3D& light_dir,
                   const Vector3D& view_dir,
                   const HitRecord& hit) const override;

    bool scatter(const Ray& r_in, const HitRecord& hit, Color& attenuation, Ray& scattered) const override;

private:
    std::shared_ptr<ABSDF> _base;
    std::shared_ptr<ITexture> _normal_map;

    Vector3D get_perturbed_normal(const HitRecord& hit) const {

        Color n_color = _normal_map->value(hit.u, hit.v, hit.point);
        Vector3D local_n(2.0 * n_color.r - 1.0, 2.0 * n_color.g - 1.0, 2.0 * n_color.b - 1.0);

        Vector3D up = std::fabs(hit.normal.y) < 0.999 ? Vector3D(0, 1, 0) : Vector3D(1, 0, 0);
        Vector3D tangent = up.cross(hit.normal);
        if (tangent.lengthSquared() < 1e-12) {
            tangent = Vector3D(1, 0, 0).cross(hit.normal);
        }
        tangent = (tangent - hit.normal * hit.normal.dot(tangent)).normalized();
        Vector3D bitangent = hit.normal.cross(tangent).normalized();

        return (tangent * local_n.x + bitangent * local_n.y + hit.normal * local_n.z).normalized();
    }
};

} // namespace Raytracer
