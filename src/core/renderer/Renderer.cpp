#include "Renderer.hpp"
#include "components/IMaterial.hpp"
#include "math/Color.hpp"
#include "math/MathUtils.hpp"
#include "math/Vector3D.hpp"

#include <thread>

namespace Raytracer {

void Renderer::render(const ICamera& camera, const Scene& scene, FrameBuffer& buffer) {
    int width = camera.getWidth();
    int height = camera.getHeight();

    _total_rows = height;
    _is_rendering = false;
    _completed_rows = 0;

    buffer.assign(width * height, Color(0, 0, 0));

    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0)
        num_threads = 4;

    std::vector<std::jthread> workers;

    int rows_per_thread = height / num_threads;

    for (unsigned int t = 0; t < num_threads; ++t) {
        int start_y = t * rows_per_thread;
        int end_y = (t == num_threads - 1) ? height : start_y + rows_per_thread;

        workers.emplace_back([this, start_y, end_y, width, height, &camera, &scene, &buffer]() {
            for (int y = start_y; y < end_y; ++y) {
                for (int x = 0; x < width; ++x) {
                    buffer[y * width + x] = samplePixel(x, y, width, height, camera, scene);
                }
                this->_completed_rows++;
            }
        });
    }

    workers.clear();
    _is_rendering = false;
}

Color Renderer::samplePixel(
    int x, int y, int width, int height, const ICamera& camera, const Scene& scene) {

    if (_samples < minimum_sampling) {
        return renderFullBatch(x, y, width, height, camera, scene, _samples);
    }

    double min_lum = 1e10, max_lum = -1e10;
    Color initial_color(0, 0, 0);

    for (int i = 0; i < initial_samples; ++i) {
        Color c = traceSingleRay(x, y, width, height, camera, scene);
        initial_color += c;

        double lum = 0.2126 * c.r + 0.7152 * c.g + 0.0722 * c.b;
        if (lum < min_lum)
            min_lum = lum;
        if (lum > max_lum)
            max_lum = lum;
    }

    double diff = max_lum - min_lum;

    if (diff < _adaptiveThreshold) {
        return initial_color / static_cast<double>(initial_samples);
    }

    Color remaining_color =
        renderFullBatch(x, y, width, height, camera, scene, _samples - initial_samples);

    return (initial_color + remaining_color) / static_cast<double>(_samples);
}

Color Renderer::traceSingleRay(
    int x, int y, int width, int height, const ICamera& camera, const Scene& scene) {
    double u_offset = Math::randomDouble(-anti_aliasing_interval, anti_aliasing_interval);
    double v_offset = Math::randomDouble(-anti_aliasing_interval, anti_aliasing_interval);

    double u = (static_cast<double>(x) + u_offset) / (width - 1.0);
    double v = 1.0 - (static_cast<double>(y) + v_offset) / (height - 1.0);

    Ray r = camera.getRay(u, v);
    return computeRayColor(r, scene, _maxDepth);
}

Color Renderer::renderFullBatch(
    int x, int y, int width, int height, const ICamera& camera, const Scene& scene, int count) {
    Color sum(0, 0, 0);
    for (int i = 0; i < count; ++i) {
        sum += traceSingleRay(x, y, width, height, camera, scene);
    }
    return sum;
}

Color Renderer::computeRayColor(const Ray& r, const Scene& scene, int depth) {
    if (depth <= 0)
        return Color(0, 0, 0);

    HitRecord rec;
    if (!scene.getWorld().hit(r, Interval(0.001, Interval::universe.max), rec)) {
        if (r.type() == RayType::CAMERA || r.type() == RayType::REFLECT) {
            return scene.getSky().getBackgroundColor(r);
        }
        return scene.getSky().getEnvironmentColor(r);
    }

    if (!rec.material)
        return Color(0, 0, 0);

    const IBSDF& bsdf = rec.material->getBSDF();

    Color ambient = computeAmbientOcclusion(r, rec, scene, bsdf, _ao_samples);

    Color color_emitted = bsdf.emitted(rec.u, rec.v, rec.point);
    Color direct = computeDirectLighting(r, rec, scene, bsdf);

    Ray scattered;
    Color attenuation;
    Color indirect(0, 0, 0);

    if (bsdf.scatter(r, rec, attenuation, scattered)) {
        indirect = attenuation * computeRayColor(scattered, scene, depth - 1);
    }

    return color_emitted + direct + indirect + ambient;
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

        double visibility = 0.0;
        for (int i = 0; i < _shadow_samples; ++i) {
            Vector3D perturbed_dir = sample.direction;
            if (_shadow_samples > 1) {
                Vector3D random_offset = Vector3D::getRandomUnitVector() * 0.05;
                perturbed_dir = (sample.direction + random_offset).normalized();
            }

            Ray shadow_ray(rec.point + rec.normal * 0.001, perturbed_dir, RayType::SHADOW);

            HitRecord shadow_rec;
            if (!scene.getWorld().hit(shadow_ray, Interval(0.001, sample.distance), shadow_rec)) {
                visibility += 1.0;
            }
        }
        visibility /= _shadow_samples;

        if (visibility > 0.0) {
            Color f = bsdf.evaluate(sample.direction, view_dir, rec);
            total_direct_light += sample.color * f * visibility;
        }
    }
    return total_direct_light;
}

Color Renderer::computeAmbientOcclusion(
    const Ray& r, const HitRecord& rec, const Scene& scene, const IBSDF& bsdf, int samples) {
    if (samples <= 0)
        return Color(0, 0, 0);

    Ray dummy_scattered;
    Color albedo(1, 1, 1);
    bsdf.scatter(r, rec, albedo, dummy_scattered);

    double occlusion = 0.0;
    for (int i = 0; i < samples; ++i) {
        Vector3D random_dir = Vector3D::randomCosineHemisphere(rec.normal);
        Ray ao_ray(rec.point + rec.normal * 0.001, random_dir, RayType::AMBIENT_OCCLUSION);

        HitRecord ao_rec;
        if (!scene.getWorld().hit(ao_ray, Interval(0.001, _ao_max_distance), ao_rec)) {
            occlusion += 1.0;
        }
    }

    const double ao_factor = occlusion / samples;
    Color sky_color = scene.getSky().getEnvironmentColor(Ray(rec.point, rec.normal));
    return sky_color * albedo * ao_factor * 0.15;
}

} // namespace Raytracer
