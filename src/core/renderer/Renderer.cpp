#include "Renderer.hpp"

namespace Raytracer
{

void Renderer::render(const ICamera& camera, const Scene& scene, FrameBuffer& buffer)
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
            pixel_color += ray_color(r, scene, 50);
            buffer[y * width + x] = pixel_color;
        }
    }
}

Color Renderer::ray_color(const Ray& r, const Scene& scene, int depth)
{
    if (depth <= 0)
        return Color(0, 0, 0);

    HitRecord rec;
    if (scene.getWorld().hit(r, Interval(0.001, std::numeric_limits<double>::infinity()), rec)) {
        
        return 0.5 * (Color(rec.normal.x, rec.normal.y, rec.normal.z) + Color(1, 1, 1));
    }

    return scene.getBackground(r);
}

} // namespace Raytracer
