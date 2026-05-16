#include "FastRenderer.hpp"
#include "components/IBSDF.hpp"
#include "components/ICamera.hpp"
#include "components/IMaterial.hpp"
#include <algorithm>
#include <cmath>
#include <random>
#include <thread>

namespace Raytracer {

FastRenderer::FastRenderer(const ISetting& settings)
    : _samples(std::max(1, settings.getInt("samples", 1))),
      _maxDepth(std::max(1, settings.getInt("max_depth", 2))) {}

void FastRenderer::render(const Scene& scene,
                          FrameBuffer& buffer,
                          std::vector<std::uint8_t>* completedRows, int startY, int endY) {
    const ICamera& camera = scene.getCamera();
    const int width = camera.getWidth();
    const int height = camera.getHeight();

    int start_y_bound = (startY < 0) ? 0 : startY;
    int end_y_bound = (endY < 0 || endY > height) ? height : endY;
    _totalRows = end_y_bound - start_y_bound;
    _completedRows = 0;
    _isRendering = true;
    _stopRequest = false;

    if (buffer.size() != static_cast<size_t>(width * height)) {
        buffer.assign(width * height, Color(0, 0, 0));
    }

    unsigned int threadCount = std::thread::hardware_concurrency();
    if (threadCount == 0) {
        threadCount = 4;
    }

    std::vector<std::jthread> workers;
    int render_height = end_y_bound - start_y_bound;
    const int rowsPerThread = std::max(1, render_height / static_cast<int>(threadCount));

    for (unsigned int t = 0; t < threadCount; ++t) {
        const int startY = static_cast<int>(t) * rowsPerThread;
        const int endY = (t == threadCount - 1) ? height : std::min(height, startY + rowsPerThread);
        const unsigned int seed = std::random_device{}() ^ static_cast<unsigned int>(t + 17);

        workers.emplace_back([this,
                              startY,
                              endY,
                              width,
                              height,
                              &camera,
                              &scene,
                              &buffer,
                              seed,
                              completedRows]() mutable {
            std::mt19937 rng(seed);
            for (int y = startY; y < endY; ++y) {
                if (_stopRequest.load()) {
                    break;
                }
                for (int x = 0; x < width; ++x) {
                    if (_stopRequest.load()) {
                        break;
                    }
                    const Color pixel = sampleFastPixel(x, y, width, height, camera, scene, rng);
                    std::scoped_lock<std::mutex> lock(getFrameBufferWriteMutex());
                    buffer[y * width + x] = pixel;
                }
                if (completedRows && y >= 0 && y < static_cast<int>(completedRows->size())) {
                    (*completedRows)[y] = 1;
                }
                ++_completedRows;
            }
        });
    }

    workers.clear();
    _isRendering = false;
}

Color FastRenderer::sampleFastPixel(int x,
                                    int y,
                                    int width,
                                    int height,
                                    const ICamera& camera,
                                    const Scene& scene,
                                    std::mt19937& rng) const {
    std::uniform_real_distribution<double> jitter(-0.18, 0.18);
    static const Vector3D previewLightDir = Vector3D(0.35, 0.85, 0.40).normalized();

    Color sum(0, 0, 0);
    for (int i = 0; i < _samples; ++i) {
        const double u = (static_cast<double>(x) + 0.5 + jitter(rng)) / std::max(1.0, width - 1.0);
        const double v =
            1.0 - (static_cast<double>(y) + 0.5 + jitter(rng)) / std::max(1.0, height - 1.0);

        Ray ray = camera.getRay(u, v);

        HitRecord rec;
        if (!scene.getWorld().hit(ray, Interval(0.001, Interval::universe.max), rec)) {
            sum += scene.getSky().getBackgroundColor(ray);
            continue;
        }

        if (!rec.material) {
            sum += 0.15 * Color(rec.normal.x + 1.0, rec.normal.y + 1.0, rec.normal.z + 1.0);
            continue;
        }

        const IBSDF& bsdf = rec.material->getBSDF();
        Color preview = bsdf.emitted(rec.u, rec.v, rec.point);
        preview += bsdf.evaluate(previewLightDir, -ray.direction().normalized(), rec) * 4.0;

        Color attenuation;
        Ray scattered;
        if (bsdf.scatter(ray, rec, attenuation, scattered)) {
            preview += 0.65 * attenuation;
        }

        const Color normalTint =
            0.25 * Color(rec.normal.x + 1.0, rec.normal.y + 1.0, rec.normal.z + 1.0);
        sum += preview + normalTint;
    }

    return stylizePreview(sum / static_cast<double>(_samples), x, y);
}

Color FastRenderer::stylizePreview(const Color& color, int x, int y) const {
    auto toneMap = [](double v) {
        v = std::max(0.0, v);
        v = v / (1.0 + v);
        return std::pow(std::clamp(v, 0.0, 1.0), 0.85);
    };

    Color draft = color;
    draft.r = toneMap(draft.r);
    draft.g = toneMap(draft.g);
    draft.b = toneMap(draft.b);

    const double scanline = ((y & 1) == 0) ? 1.0 : 0.96;
    const double block = (((x >> 2) + (y >> 2)) & 1) ? 1.0 : 0.985;
    draft *= scanline * block;

    return draft;
}

extern "C" {

const char* getName() { return "fast"; }

IRenderer* createPlugin(const ISetting& settings) { return new FastRenderer(settings); }

} // extern "C"

} // namespace Raytracer
