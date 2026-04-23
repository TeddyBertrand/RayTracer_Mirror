#include "Renderer.hpp"
#include "components/IMaterial.hpp"
#include "math/Color.hpp"

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
    if (scene.getWorld().hit(r, Interval(0.001, std::numeric_limits<double>::max()), rec)) {
        Ray scattered;
        Color attenuation;

        Color emitted = rec.material->emit(rec);

        if (rec.material->scatter(r, rec, attenuation, scattered)) {
            double specWeight = rec.material->getSpecularWeight();
            double diffuseWeight = 1.0 - specWeight;

            Color direct_light(0, 0, 0);
            if (diffuseWeight > 0) {
                direct_light = computeDirectLighting(rec, scene, attenuation) * diffuseWeight;
            }
            return emitted + direct_light + attenuation * computeRayColor(scattered, scene, depth - 1, false);
        }
        return emitted;
    }

    return isFirstRay ? scene.getSky().getBackgroundColor(r) : scene.getSky().getEnvironmentColor(r);
}

Color Renderer::computeDirectLighting(const HitRecord& rec, const Scene& scene, const Color& attenuation)
{
    Color total_direct_light(0, 0, 0);

    for (const auto& light : scene.getLights()) {
        LightSample sample = light->computeLight(rec.point);
        if (!sample.isActive) continue;

        Ray shadow_ray(rec.point + rec.normal * 0.001, sample.direction);
        Color light_visibility(1.0, 1.0, 1.0);
        double t_min = 0.001;

        HitRecord shadow_rec;
        while (scene.getWorld().hit(shadow_ray, Interval(t_min, sample.distance), shadow_rec)) {
            Color transmittance = shadow_rec.material->getTransmittance();
            
            light_visibility *= transmittance;

            if (light_visibility.isNearZero()) {
                light_visibility = Color(0, 0, 0);
                break;
            }

            t_min = shadow_rec.t + 0.001;
        }

        if (!light_visibility.isNearZero()) {
            double cos_theta = std::max(0.0, rec.normal.dot(sample.direction));
            total_direct_light += (attenuation * sample.color * light_visibility) * cos_theta;
        }
    }
    return total_direct_light;
}

} // namespace Raytracer
