#include "Renderer.hpp"
#include "components/IMaterial.hpp"

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

Color Renderer::ray_color(const Ray& r, const Scene& scene, int depth) {
    if (depth <= 0) return Color(0, 0, 0);

    HitRecord rec;
    if (scene.getWorld().hit(r, Interval(0.001, Interval::universe.max), rec)) {
        Ray scattered;
        Color attenuation;

        if (rec.material->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, scene, depth - 1);
        }
        return Color(0, 0, 0);
    }

    return scene.getBackground(r);
}

} // namespace Raytracer
