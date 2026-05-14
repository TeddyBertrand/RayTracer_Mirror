#include "Renderer.hpp"
#include "components/IMaterial.hpp"

#include <cmath>
#include <random>
#include <thread>

namespace Raytracer {
namespace {

double randomDouble(std::mt19937& rng, double min, double max) {
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(rng);
}

Vector3D randomUnitVector(std::mt19937& rng) {
    while (true) {
        Vector3D p(randomDouble(rng, -1.0, 1.0),
                   randomDouble(rng, -1.0, 1.0),
                   randomDouble(rng, -1.0, 1.0));
        double lensq = p.lengthSquared();
        if (lensq > 1e-160 && lensq <= 1.0) {
            return p / std::sqrt(lensq);
        }
    }
}

Vector3D randomCosineHemisphere(std::mt19937& rng, const Vector3D& normal) {
    double r1 = randomDouble(rng, 0.0, 1.0);
    double r2 = randomDouble(rng, 0.0, 1.0);
    double phi = 2.0 * M_PI * r1;
    double x = std::cos(phi) * std::sqrt(r2);
    double y = std::sin(phi) * std::sqrt(r2);
    double z = std::sqrt(std::max(0.0, 1.0 - r2));

    Vector3D w = normal.normalized();
    Vector3D a = (std::fabs(w.x) > 0.9) ? Vector3D(0, 1, 0) : Vector3D(1, 0, 0);
    Vector3D v = w.cross(a).normalized();
    Vector3D u = v.cross(w);

    Vector3D res;
    res.x = u.x * x + v.x * y + w.x * z;
    res.y = u.y * x + v.y * y + w.y * z;
    res.z = u.z * x + v.z * y + w.z * z;
    return res.normalized();
}

} // namespace

Renderer::Renderer(const ISetting& settings)
    : _samples(settings.getInt("samples", 10)), _maxDepth(settings.getInt("max_depth", 50)),
      _adaptiveThreshold(settings.getFloat("adaptive_threshold", 0.1)),
      _ao_samples(settings.getInt("ao_samples", 0)),
      _ao_max_distance(settings.getFloat("ao_max_distance", 10.0)),
      _shadow_samples(settings.getInt("shadow_samples", 4)) {}

void Renderer::render(const Scene& scene,
                      FrameBuffer& buffer,
                      std::vector<std::uint8_t>* completedRows) {
    const ICamera& camera = scene.getCamera();
    int width = camera.getWidth();
    int height = camera.getHeight();

    _total_rows = height;
    _is_rendering = true;
    _completed_rows = 0;
    _stopRequest = false;

    if (buffer.size() != static_cast<size_t>(width * height)) {
        buffer.assign(width * height, Color(0, 0, 0));
    }

    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0)
        num_threads = 4;

    std::vector<std::jthread> workers;
    int rows_per_thread = height / num_threads;

    for (unsigned int t = 0; t < num_threads; ++t) {
        int start_y = t * rows_per_thread;
        int end_y = (t == num_threads - 1) ? height : start_y + rows_per_thread;
        unsigned int seed = std::random_device{}() ^ static_cast<unsigned int>(t + 1);

        workers.emplace_back([this,
                              start_y,
                              end_y,
                              width,
                              height,
                              &camera,
                              &scene,
                              &buffer,
                              seed,
                              completedRows]() mutable {
            std::mt19937 rng(seed);
            for (int y = start_y; y < end_y; ++y) {
                if (_stopRequest.load()) {
                    break;
                }
                for (int x = 0; x < width; ++x) {
                    if (_stopRequest.load()) {
                        break;
                    }
                    const Color pixel = samplePixel(x, y, width, height, camera, scene, rng);
                    std::scoped_lock<std::mutex> lock(getFrameBufferWriteMutex());
                    buffer[y * width + x] = pixel;
                }
                if (completedRows && y >= 0 && y < static_cast<int>(completedRows->size())) {
                    (*completedRows)[y] = 1;
                }
                ++_completed_rows;
            }
        });
    }

    workers.clear();
    _is_rendering = false;
}

Color Renderer::samplePixel(int x,
                            int y,
                            int width,
                            int height,
                            const ICamera& camera,
                            const Scene& scene,
                            std::mt19937& rng) {
    if (_samples < minimum_sampling) {
        return renderFullBatch(x, y, width, height, camera, scene, _samples, rng);
    }

    double min_lum = 1e10;
    double max_lum = -1e10;
    Color initial_color(0, 0, 0);

    for (int i = 0; i < initial_samples; ++i) {
        Color c = traceSingleRay(x, y, width, height, camera, scene, rng);
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
        renderFullBatch(x, y, width, height, camera, scene, _samples - initial_samples, rng);

    return (initial_color + remaining_color) / static_cast<double>(_samples);
}

Color Renderer::traceSingleRay(int x,
                               int y,
                               int width,
                               int height,
                               const ICamera& camera,
                               const Scene& scene,
                               std::mt19937& rng) {
    std::uniform_real_distribution<double> jitter(-anti_aliasing_interval, anti_aliasing_interval);
    double u_offset = jitter(rng);
    double v_offset = jitter(rng);

    double u = (static_cast<double>(x) + u_offset) / (width - 1.0);
    double v = 1.0 - (static_cast<double>(y) + v_offset) / (height - 1.0);

    Ray r = camera.getRay(u, v);
    return computeRayColor(r, scene, _maxDepth, rng);
}

Color Renderer::renderFullBatch(int x,
                                int y,
                                int width,
                                int height,
                                const ICamera& camera,
                                const Scene& scene,
                                int count,
                                std::mt19937& rng) {
    Color sum(0, 0, 0);
    for (int i = 0; i < count; ++i) {
        sum += traceSingleRay(x, y, width, height, camera, scene, rng);
    }
    return sum;
}

Color Renderer::computeRayColor(const Ray& r, const Scene& scene, int depth, std::mt19937& rng) {
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

    Color ambient = computeAmbientOcclusion(r, rec, scene, bsdf, _ao_samples, rng);
    Color color_emitted = bsdf.emitted(rec.u, rec.v, rec.point);
    Color direct = computeDirectLighting(r, rec, scene, bsdf, rng);

    Ray scattered;
    Color attenuation;
    Color indirect(0, 0, 0);

    if (bsdf.scatter(r, rec, attenuation, scattered)) {
        indirect = attenuation * computeRayColor(scattered, scene, depth - 1, rng);
    }

    return color_emitted + direct + indirect + ambient;
}

Color Renderer::computeDirectLighting(const Ray& r_in,
                                      const HitRecord& rec,
                                      const Scene& scene,
                                      const IBSDF& bsdf,
                                      std::mt19937& rng) {
    Color total_direct_light(0, 0, 0);
    Vector3D view_dir = -r_in.direction();

    for (const auto& light : scene.getLights()) {

        LightSample sample = light->computeLight(rec);
        if (!sample.isActive)
            continue;

        double visibility = 0.0;
        for (int i = 0; i < _shadow_samples; ++i) {
            Vector3D perturbed_dir = sample.direction;
            if (_shadow_samples > 1) {
                Vector3D random_offset = randomUnitVector(rng) * 0.05;
                perturbed_dir = (sample.direction + random_offset).normalized();
            }

            Ray shadow_ray(rec.point + rec.normal * 0.001, perturbed_dir, RayType::SHADOW);
            HitRecord shadow_rec;
            if (!scene.getWorld().hit(shadow_ray, Interval(0.001, sample.distance), shadow_rec)) {
                visibility += 1.0;
            }
        }

        visibility /= _shadow_samples;
        if (visibility <= 0.0)
            continue;

        Color f = bsdf.evaluate(sample.direction, view_dir, rec);
        total_direct_light += sample.color * f * visibility;
    }

    return total_direct_light;
}

Color Renderer::computeAmbientOcclusion(const Ray& r,
                                        const HitRecord& rec,
                                        const Scene& scene,
                                        const IBSDF& bsdf,
                                        int samples,
                                        std::mt19937& rng) {
    if (samples <= 0)
        return Color(0, 0, 0);

    Ray dummy_scattered;
    Color albedo(1, 1, 1);
    bsdf.scatter(r, rec, albedo, dummy_scattered);

    double occlusion = 0.0;
    for (int i = 0; i < samples; ++i) {
        Vector3D random_dir = randomCosineHemisphere(rng, rec.normal);
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

extern "C" {

const char* getName() { return "default"; }

IRenderer* createPlugin(const ISetting& settings) { return new Renderer(settings); }

} // extern "C"

} // namespace Raytracer
