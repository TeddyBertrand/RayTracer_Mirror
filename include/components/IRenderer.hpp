#pragma once

#include "core/scene/Scene.hpp"
#include "render/FrameBuffer.hpp"
#include <cstdint>
#include <vector>

namespace Raytracer {

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void render(const Scene& scene,
                        FrameBuffer& buffer,
                        std::vector<std::uint8_t>* completedRows = nullptr, int startY = 0, int endY = -1) = 0;

    virtual int getCompletedRows() const = 0;
    virtual int getTotalRows() const = 0;
    virtual bool isRendering() const = 0;

    /**
     * @brief Request the renderer to stop rendering gracefully
     */
    virtual void stop() = 0;

    /**
     * @brief Check if a stop request has been made
     */
    virtual bool shouldStop() const = 0;
};

} // namespace Raytracer
