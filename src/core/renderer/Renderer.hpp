#pragma once

#include <vector>
#include <memory>
#include "components/ICamera.hpp"
#include "components/IPrimitive.hpp"
#include "core/scene/Scene.hpp"
#include "render/FrameBuffer.hpp"


namespace Raytracer
{

class Renderer {
public:
    Renderer(int samples = 10, int depth = 50) 
        : _samples(samples), _maxDepth(depth) {}
    ~Renderer() = default;

    void render(const ICamera& camera, const Scene& scene, FrameBuffer& buffer);

    void setSamples(int samples) { _samples = samples; }
    void setMaxDepth(int depth) { _maxDepth = depth; }

private:
    static constexpr float anti_aliasing_interval = 0.5f;

private:
    int _samples;
    int _maxDepth;
    Color computeRayColor(const Ray& r, const Scene& scene, int depth, bool isFirstRay = true);
    Color computeDirectLighting(const HitRecord& rec, const Scene& scene, const Color& attenuation);
};

} // namespace Raytracer
