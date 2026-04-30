#pragma once

#include "components/ICamera.hpp"
#include "components/IMaterial.hpp"
#include "components/IPrimitive.hpp"
#include "core/scene/Scene.hpp"
#include "math/Color.hpp"
#include "render/FrameBuffer.hpp"
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

private:
    static constexpr float anti_aliasing_interval = 0.5f;

private:
    int _samples;
    int _maxDepth;
    Color computeRayColor(const Ray& r, const Scene& scene, int depth);
    Color
    samplePixel(int x, int y, int width, int height, const ICamera& camera, const Scene& scene);
    Color computeDirectLighting(const Ray& r_in,
                                const HitRecord& rec,
                                const Scene& scene,
                                const IBSDF& bsdf);
};

} // namespace Raytracer
