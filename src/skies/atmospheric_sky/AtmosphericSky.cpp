#include "AtmosphericSky.hpp"

namespace Raytracer
{

Color AtmosphericSky::getBackgroundColor(const Ray& r) const
{
    Vector3D unit_direction = r.direction().normalized();

    double t = 0.5 * (unit_direction.y + 1.0);

    return _groundColor * (1.0 - t) + _zenithColor * t;
}

} // namespace Raytracer
