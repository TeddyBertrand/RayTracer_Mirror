#pragma once

#include <vector>
#include <memory>
#include "PrimitiveList.hpp"
#include "components/ILight.hpp"
#include "math/Color.hpp"
#include "math/Ray.hpp"

namespace Raytracer {

class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    void addPrimitive(std::shared_ptr<IPrimitive> primitive) {
        _world.add(primitive);
    }

    void addLight(std::shared_ptr<ILight> light) {
        _lights.push_back(light);
    }

    void setBackgroundColor(const Color& color) { _backgroundColor = color; }
    void setAmbientLight(const Color& color) { _ambientLight = color; }

    const IPrimitive& getWorld() const { return _world; }
    const std::vector<std::shared_ptr<ILight>>& getLights() const { return _lights; }
    
    Color getBackground(const Ray& r) const;

private:
    PrimitiveList _world;
    
    std::vector<std::shared_ptr<ILight>> _lights;
    
    Color _backgroundColor = Color(0.5, 0.7, 1.0);
    Color _ambientLight = Color(0.1, 0.1, 0.1);
};

} // namespace Raytracer
