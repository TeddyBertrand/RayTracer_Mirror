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

            Ray r = camera.getRay(u, v);
            pixel_color += computeRayColor(r, scene, 50);
            buffer[y * width + x] = pixel_color;
        }
    }
}

Color Renderer::computeRayColor(const Ray& r, const Scene& scene, int depth, bool isFirstRay) {
    if (depth <= 0) return Color(0, 0, 0);

    HitRecord rec;
    if (scene.getWorld().hit(r, Interval(0.001, Interval::universe.max), rec)) {
        Ray scattered;
        Color attenuation;

        if (rec.material->scatter(r, rec, attenuation, scattered)) {
            Color direct_light = computeDirectLighting(rec, scene, attenuation);

            return direct_light + attenuation * computeRayColor(scattered, scene, depth - 1, false);
        }
        return Color(0, 0, 0);
    }

    if (!isFirstRay) {
        return scene.getSky().getEnvironmentColor(r);
    }
    return scene.getSky().getBackgroundColor(r);
}

Color Renderer::computeDirectLighting(const HitRecord& rec, const Scene& scene, const Color& attenuation)
{
    Color direct_light(0, 0, 0);
    for (const auto& light : scene.getLights()) {
        LightSample sample = light->computeLight(rec.point);
        if (!sample.isActive) continue;

        Ray shadow_ray(rec.point + rec.normal * 0.001, sample.direction);
        HitRecord shadow_rec;

        if (!scene.getWorld().hit(shadow_ray, Interval(0.001, sample.distance), shadow_rec)) {
            double cos_theta = std::max(0.0, rec.normal.dot(sample.direction));
            direct_light += (attenuation * sample.color) * cos_theta;
        }
    }
    return direct_light;
}

} // namespace Raytracer
