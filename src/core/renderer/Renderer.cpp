#include "Renderer.hpp"
#include "components/IMaterial.hpp"
#include "math/Color.hpp"
#include "math/MathUtils.hpp"

namespace Raytracer
{

void Renderer::render(const ICamera& camera, const Scene& scene, FrameBuffer& buffer)
{
    int width = camera.getWidth();
    int height = camera.getHeight();

    buffer.assign(width * height, Color(0, 0, 0));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Color pixel_color(0, 0, 0);
            int sqrt_samples = std::sqrt(_samples);

            for (int s_y = 0; s_y < sqrt_samples; ++s_y) {
                for (int s_x = 0; s_x < sqrt_samples; ++s_x) {
                    double u_offset = (s_x + Math::randomDouble(0, 1)) / sqrt_samples - anti_aliasing_interval;
                    double v_offset = (s_y + Math::randomDouble(0, 1)) / sqrt_samples - anti_aliasing_interval;

                    double u = (static_cast<double>(x) + u_offset) / (width - 1.0);
                    double v = 1.0 - (static_cast<double>(y) + v_offset) / (height - 1.0);

                    Ray r = camera.getRay(u, v);
                    pixel_color += computeRayColor(r, scene, 50);
                }
            }
            buffer[y * width + x] = pixel_color / static_cast<double>(_samples);
        }
    }
}

Color Renderer::computeRayColor(const Ray& r, const Scene& scene, int depth, bool isFirstRay) {
    if (depth <= 0) return Color(0, 0, 0);

    HitRecord rec;
    if (!scene.getWorld().hit(r, Interval(0.001, std::numeric_limits<double>::max()), rec)) {
        return isFirstRay ? scene.getSky().getBackgroundColor(r) : scene.getSky().getEnvironmentColor(r);
    }

    Color emission = (isFirstRay) ? rec.material->emit(rec) : Color(0,0,0);

    Ray scattered;
    Color attenuation;
    if (rec.material->scatter(r, rec, attenuation, scattered)) {        
        Color direct = computeDirectLighting(rec, scene, attenuation);

        double diffuseWeight = 1.0 - rec.material->getSpecularWeight();
        Color indirect = attenuation * computeRayColor(scattered, scene, depth - 1, false);

        return emission + (direct * diffuseWeight) + indirect;
    }

    return emission;
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
