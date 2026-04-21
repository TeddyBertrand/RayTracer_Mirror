#include "Renderer.hpp"

namespace Raytracer
{

void Renderer::render(const ICamera& camera, const IPrimitive& world, FrameBuffer& buffer)
{
    int width = camera.getWidth();
    int height = camera.getHeight();

    buffer.assign(width * height, Color(0, 0, 0));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Color pixel_color;

            double u = static_cast<double>(x) / (width - 1.0);
            double v = 1.0 - static_cast<double>(y) / (height - 1.0);

            Ray r = camera.get_ray(u, v);
            pixel_color += ray_color(r, world, 50);
            buffer[y * width + x] = pixel_color;
        }
    }
}

Color Renderer::ray_color(const Ray& r, const IPrimitive& world, int depth)
{
    if (depth <= 0)
        return Color(0, 0, 0);

    HitRecord rec;
    if (world.hit(r, Interval(0.001, Interval::universe.max), rec)) {
        return 0.5 * (Color(rec.normal.x, rec.normal.y, rec.normal.z) + Color(1, 1, 1));
    }

    Vector3D unit_direction = r.direction().normalized();
    double t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * Color(1, 1, 1) + t * Color(0.5, 0.7, 1.0);
}

} // namespace Raytracer
