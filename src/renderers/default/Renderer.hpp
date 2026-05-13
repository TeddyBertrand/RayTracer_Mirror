#pragma once

#include "components/IBSDF.hpp"
#include "components/ICamera.hpp"
#include "components/IMaterial.hpp"
#include "components/IRenderer.hpp"
#include "core/scene/Scene.hpp"
#include "math/Color.hpp"
#include "parser/ISettings.hpp"
#include "render/FrameBuffer.hpp"
#include <atomic>
#include <memory>
#include <random>
#include <vector>

namespace Raytracer {

class Renderer : public IRenderer {
public:
    explicit Renderer(const ISetting& settings);
    ~Renderer() override = default;

    void render(const Scene& scene,
                FrameBuffer& buffer,
                std::vector<std::uint8_t>* completedRows = nullptr) override;

    int getCompletedRows() const override { return _completed_rows.load(); }
    int getTotalRows() const override { return _total_rows; }
    bool isRendering() const override { return _is_rendering.load(); }

    void stop() override { _stopRequest.store(true); }
    bool shouldStop() const override { return _stopRequest.load(); }

private:
    static constexpr float anti_aliasing_interval = 0.5f;
    static constexpr int initial_samples = 4;
    static constexpr int minimum_sampling = 8;

private:
    int _samples;
    int _maxDepth;
    double _adaptiveThreshold = 0.1;
    int _ao_samples = 0;
    double _ao_max_distance = 10.0;
    int _shadow_samples = 4;

    std::atomic<int> _completed_rows{0};
    std::atomic<bool> _is_rendering{false};
    std::atomic<bool> _stopRequest{false};
    int _total_rows{0};

private:
    Color computeRayColor(const Ray& r, const Scene& scene, int depth, std::mt19937& rng);

    Color computeDirectLighting(const Ray& r_in,
                                const HitRecord& rec,
                                const Scene& scene,
                                const IBSDF& bsdf,
                                std::mt19937& rng);

    Color computeAmbientOcclusion(const Ray& r,
                                  const HitRecord& rec,
                                  const Scene& scene,
                                  const IBSDF& bsdf,
                                  int samples,
                                  std::mt19937& rng);

    Color samplePixel(int x,
                      int y,
                      int width,
                      int height,
                      const ICamera& camera,
                      const Scene& scene,
                      std::mt19937& rng);
    Color renderFullBatch(int x,
                          int y,
                          int width,
                          int height,
                          const ICamera& camera,
                          const Scene& scene,
                          int count,
                          std::mt19937& rng);
    Color traceSingleRay(int x,
                         int y,
                         int width,
                         int height,
                         const ICamera& camera,
                         const Scene& scene,
                         std::mt19937& rng);
};

} // namespace Raytracer
