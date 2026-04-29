#pragma once

#include "PrimitiveList.hpp"
#include "components/ICamera.hpp"
#include "components/ILight.hpp"
#include "components/ISky.hpp"
#include "math/Color.hpp"
#include "math/Ray.hpp"
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace Raytracer {

class Scene {
public:
    Scene() = default;
    ~Scene() = default;

    // --- Matériaux ---
    void addMaterial(const std::string& name, std::shared_ptr<IMaterial> material) {
        _materials[name] = material;
    }

    std::shared_ptr<IMaterial> getMaterial(const std::string& name) const {
        auto it = _materials.find(name);
        return (it != _materials.end()) ? it->second : nullptr;
    }

    const std::unordered_map<std::string, std::shared_ptr<IMaterial>>& getMaterials() const {
        return _materials;
    }

    // --- Géométrie ---
    void addPrimitive(std::shared_ptr<IPrimitive> primitive) { _world.add(primitive); }
    const IPrimitive& getWorld() const { return _world; }

    // --- Lumières ---
    void addLight(std::shared_ptr<ILight> light) { _lights.push_back(light); }
    const std::vector<std::shared_ptr<ILight>>& getLights() const { return _lights; }

    // --- Environnement & Caméra ---
    void setBackgroundColor(const Color& color) { _backgroundColor = color; }
    void setSky(std::shared_ptr<ISky> sky) { _sky = std::move(sky); }
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
    void dump();

private:
    PrimitiveList _world;
    std::vector<std::shared_ptr<ILight>> _lights;
    std::unordered_map<std::string, std::shared_ptr<IMaterial>> _materials;
    std::shared_ptr<ISky> _sky;
    std::shared_ptr<ICamera> _camera;

    Color _backgroundColor = Color(0.5, 0.7, 1.0);
    Color _ambientLight = Color(0.1, 0.1, 0.1);
};
} // namespace Raytracer
