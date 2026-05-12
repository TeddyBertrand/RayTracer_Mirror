#pragma once

#include "core/scene/Scene.hpp"
#include "render/FrameBuffer.hpp"

namespace Raytracer {

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void render(const Scene& scene, FrameBuffer& buffer) = 0;

    virtual int getCompletedRows() const = 0;
    virtual int getTotalRows() const = 0;
    virtual bool isRendering() const = 0;
};

} // namespace Raytracer
