#include "Renderer.hpp"
#include "components/IMaterial.hpp"
#include "math/Color.hpp"
#include "math/MathUtils.hpp"

namespace Raytracer {

void Renderer::render(const ICamera& camera, const Scene& scene, FrameBuffer& buffer) {
    int width = camera.getWidth();
    int height = camera.getHeight();

    buffer.assign(width * height, Color(0, 0, 0));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Color pixel_color(0, 0, 0);
            int sqrt_samples = std::sqrt(_samples);

            for (int s_y = 0; s_y < sqrt_samples; ++s_y) {
                for (int s_x = 0; s_x < sqrt_samples; ++s_x) {
                    double u_offset =
                        (s_x + Math::randomDouble(0, 1)) / sqrt_samples - anti_aliasing_interval;
                    double v_offset =
                        (s_y + Math::randomDouble(0, 1)) / sqrt_samples - anti_aliasing_interval;

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

Color Renderer::computeRayColor(const Ray& r, const Scene& scene, int depth) {
    if (depth <= 0)
        return Color(0, 0, 0);

    HitRecord rec;
    if (!scene.getWorld().hit(r, Interval(0.001, std::numeric_limits<double>::max()), rec)) {
        if (r.type() == RayType::CAMERA || r.type() == RayType::REFLECT) {
            return scene.getSky().getBackgroundColor(r);
        }
        return scene.getSky().getEnvironmentColor(r);
    }

    if (!rec.material) {
        return Color(0, 0, 0);
    }

    auto bsdf = rec.material->getBSDF(rec);

    Color color_emitted = bsdf->emitted(rec.u, rec.v, rec.point);

    Color direct = computeDirectLighting(r, rec, scene, *bsdf);

    Ray scattered;
    Color attenuation;
    Color indirect(0, 0, 0);

    if (bsdf->scatter(r, rec, attenuation, scattered)) {
        indirect = attenuation * computeRayColor(scattered, scene, depth - 1);
    }

    return color_emitted + direct + indirect;
}

Color Renderer::computeDirectLighting(const Ray& r_in,
                                      const HitRecord& rec,
                                      const Scene& scene,
                                      const IBSDF& bsdf) {
    Color total_direct_light(0, 0, 0);
    Vector3D view_dir = -r_in.direction();

    for (const auto& light : scene.getLights()) {
        LightSample sample = light->computeLight(rec.point);
        if (!sample.isActive)
            continue;

        Ray shadow_ray(rec.point + rec.normal * 0.001, sample.direction, RayType::SHADOW);

        HitRecord shadow_rec;
        if (scene.getWorld().hit(shadow_ray, Interval(0.001, sample.distance), shadow_rec)) {
            continue;
        }

        Color f = bsdf.evaluate(sample.direction, view_dir, rec);

        total_direct_light += sample.color * f;
    }
    return total_direct_light;
}

} // namespace Raytracer
