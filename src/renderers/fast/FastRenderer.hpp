#pragma once

#include "components/IRenderer.hpp"
#include "parser/ISettings.hpp"
#include <atomic>
#include <random>

namespace Raytracer {

class ICamera;

class FastRenderer : public IRenderer {
public:
    explicit FastRenderer(const ISetting& settings);
    ~FastRenderer() override = default;

    void render(const Scene& scene,
                FrameBuffer& buffer,
                std::vector<std::uint8_t>* completedRows = nullptr, int startY = 0, int endY = -1) override;

    int getCompletedRows() const override { return _completedRows.load(); }
    int getTotalRows() const override { return _totalRows; }
    bool isRendering() const override { return _isRendering.load(); }

    void stop() override { _stopRequest.store(true); }
    bool shouldStop() const override { return _stopRequest.load(); }

private:
    Color sampleFastPixel(int x,
                          int y,
                          int width,
                          int height,
                          const ICamera& camera,
                          const Scene& scene,
                          std::mt19937& rng) const;
    Color stylizePreview(const Color& color, int x, int y) const;

private:
    int _samples;
    int _maxDepth;

    std::atomic<int> _completedRows{0};
    std::atomic<bool> _isRendering{false};
    std::atomic<bool> _stopRequest{false};
    int _totalRows{0};
};

} // namespace Raytracer
