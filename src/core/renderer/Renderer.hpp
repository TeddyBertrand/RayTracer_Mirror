#pragma once

#include "components/ICamera.hpp"
#include "components/IMaterial.hpp"
#include "components/IPrimitive.hpp"
#include "core/scene/Scene.hpp"
#include "math/Color.hpp"
#include "render/FrameBuffer.hpp"
#include <atomic>
#include <memory>
#include <vector>

namespace Raytracer {

class Renderer {
public:
    Renderer(int samples = 10, int depth = 50) : _samples(samples), _maxDepth(depth) {}
    ~Renderer() = default;

    void render(const ICamera& camera, const Scene& scene, FrameBuffer& buffer);

    void setSamples(int samples) { _samples = samples; }
    void setMaxDepth(int depth) { _maxDepth = depth; }
    void setAdaptiveThreshold(double threshold) { _adaptiveThreshold = threshold; }

    int getCompletedRows() const { return _completed_rows.load(); }
    int getTotalRows() const { return _total_rows; }
    bool isRendering() const { return _is_rendering.load(); }

private:
    static constexpr float anti_aliasing_interval = 0.5f;
    static constexpr int initial_samples = 4;
    static constexpr int minimum_sampling = 8;

private:
    int _samples;
    int _maxDepth;
    double _adaptiveThreshold = 0.1;

    std::atomic<int> _completed_rows{0};
    std::atomic<bool> _is_rendering{false};
    int _total_rows{0};

private:
    Color computeRayColor(const Ray& r, const Scene& scene, int depth);

    Color computeDirectLighting(const Ray& r_in,
                                const HitRecord& rec,
                                const Scene& scene,
                                const IBSDF& bsdf);

    Color
    samplePixel(int x, int y, int width, int height, const ICamera& camera, const Scene& scene);
    Color renderFullBatch(
        int x, int y, int width, int height, const ICamera& camera, const Scene& scene, int count);
    Color
    traceSingleRay(int x, int y, int width, int height, const ICamera& camera, const Scene& scene);
};

} // namespace Raytracer
