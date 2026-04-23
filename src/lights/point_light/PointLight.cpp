#include "PointLight.hpp"

namespace Raytracer
{

LightSample PointLight::computeLight(const Point3D& hit_point) const
{
    Vector3D direction = (_position - hit_point);
    double distance_squared = direction.lengthSquared();
    direction.normalize();

    LightSample sample;
    double attenuation = _intensity / std::max(1.0, distance_squared);
    sample.color = _color * attenuation;
    sample.direction = direction;
    sample.distance = std::sqrt(distance_squared);
    sample.isActive = true;

    return sample;
}

} // namespace Raytracer
