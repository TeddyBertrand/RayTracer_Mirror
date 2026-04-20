#pragma once

#include <vector>
#include <memory>
#include "components/ICamera.hpp"
#include "components/IPrimitive.hpp"
#include "render/FrameBuffer.hpp"


namespace Raytracer
{

class Renderer {
public:
    Renderer(int samples = 10, int depth = 50) 
        : _samples(samples), _maxDepth(depth) {}
    ~Renderer() = default;

    void render(const ICamera& camera, const IPrimitive& world, FrameBuffer& buffer);

private:
    int _samples;
    int _maxDepth;
    Color ray_color(const Ray& r, const IPrimitive& world, int depth);
};

} // namespace Raytracer
