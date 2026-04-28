#include "Texture.hpp"

namespace Raytracer
{

bool TextureMaterial::scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const
{
    auto u = rec.u;
    auto v = rec.v;
    auto texture_color = _texture.value(u, v);

    attenuation = Color(texture_color.r * _albedo.r, texture_color.g * _albedo.g, texture_color.b * _albedo.b);
    auto scatter_direction = rec.normal;
    scattered = Ray(rec.point, scatter_direction);

    return true;
}

} // namespace Raytracer
