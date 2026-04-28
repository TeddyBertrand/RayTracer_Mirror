#pragma once

#include "PrimitiveList.hpp"
#include "components/ICamera.hpp"
#include "components/ILight.hpp"
#include "components/ISky.hpp"
#include "math/Color.hpp"
#include "math/Ray.hpp"
#include <memory>
#include <stdexcept>
#include <vector>

namespace Raytracer {

class Scene {
public:
    Scene() = default;
    ~Scene() = default;

    void addPrimitive(std::shared_ptr<IPrimitive> primitive) { _world.add(primitive); }
    const IPrimitive& getWorld() const { return _world; }

    void addLight(std::shared_ptr<ILight> light) { _lights.push_back(light); }
    const std::vector<std::shared_ptr<ILight>>& getLights() const { return _lights; }
    void setBackgroundColor(const Color& color) { _backgroundColor = color; }
    void setAmbientLight(const Color& color) { _ambientLight = color; }

    void setSky(std::unique_ptr<ISky> sky) { _sky = std::move(sky); }
    const ISky& getSky() const {
        if (!_sky)
            throw std::runtime_error("Scene: Sky is not set.");
        return *_sky;
    }

    void setCamera(std::shared_ptr<ICamera> camera) { _camera = camera; }

    const ICamera& getCamera() const {
        if (!_camera)
            throw std::runtime_error("Scene: Camera is not set.");
        return *_camera;
    }

private:
    PrimitiveList _world;
    std::vector<std::shared_ptr<ILight>> _lights;
    std::unique_ptr<ISky> _sky;

    std::shared_ptr<ICamera> _camera;

    Color _backgroundColor = Color(0.5, 0.7, 1.0);
    Color _ambientLight = Color(0.1, 0.1, 0.1);
};

} // namespace Raytracer
