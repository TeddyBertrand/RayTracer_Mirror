#include "Scene.hpp"

namespace Raytracer
{

Color Scene::getBackground(const Ray& r) const
{
    Vector3D unit_dir = r.direction().normalized();
    double t = 0.5 * (unit_dir.y + 1.0);
    return (1.0 - t) * Color(1, 1, 1) + t * _backgroundColor;
}

} // namespace Raytracer
