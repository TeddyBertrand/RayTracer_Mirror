#pragma once

#include "components/ILight.hpp"
#include "factory/AFactory.hpp"

namespace Raytracer {

/**
 * @brief Concrete factory that creates light plugins from registered callbacks.
 */
class LightFactory : public AFactory<ILight> {
public:
    /**
     * @brief Create a light instance by name.
     *
     * @return Shared pointer to the created light, or nullptr if unknown.
     */
    std::shared_ptr<ILight> create(const std::string& name, const ISetting& settings) override {
        if (_factoryMap.find(name) != _factoryMap.end()) {
            ILight* ptr = _factoryMap[name](settings);
            return ptr ? std::shared_ptr<ILight>(ptr) : nullptr;
        }
        return nullptr;
    }
};
}; // namespace Raytracer
