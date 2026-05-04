#include "Renderer.hpp"
#include "components/IMaterial.hpp"
#include "math/Color.hpp"
#include "math/MathUtils.hpp"

#include <execution>
#include <numeric>

namespace Raytracer {

void Renderer::render(const ICamera& camera, const Scene& scene, FrameBuffer& buffer) {
    int width = camera.getWidth();
    int height = camera.getHeight();

    _total_rows = height;
    _is_rendering = false;
    _completed_rows = 0;

    buffer.assign(width * height, Color(0, 0, 0));

    std::vector<int> rows(height);
    std::iota(rows.begin(), rows.end(), 0);

    std::for_each(std::execution::par, rows.begin(), rows.end(), [&](int y) {
        for (int x = 0; x < width; ++x) {
            buffer[y * width + x] = samplePixel(x, y, width, height, camera, scene);
        }
        _completed_rows++;
    });
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
    if (depth <= 0) {
        return Color(0, 0, 0);
    }

    HitRecord rec;
    if (!scene.getWorld().hit(r, Interval(0.001, Interval::universe.max), rec)) {
        if (r.type() == RayType::CAMERA || r.type() == RayType::REFLECT) {
            return scene.getSky().getBackgroundColor(r);
        }
        return scene.getSky().getEnvironmentColor(r);
    }

    if (!rec.material) {
        return Color(0, 0, 0);
    }

    const IBSDF& bsdf = rec.material->getBSDF();

    Color color_emitted = bsdf.emitted(rec.u, rec.v, rec.point);

    Color direct = computeDirectLighting(r, rec, scene, bsdf);

    Ray scattered;
    Color attenuation;
    Color indirect(0, 0, 0);

    if (bsdf.scatter(r, rec, attenuation, scattered)) {
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
