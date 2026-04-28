#pragma once

#include "components/IMaterial.hpp"
#include "factory/AFactory.hpp"

namespace Raytracer {

/**
 * @brief Concrete factory that creates material plugins from registered callbacks.
 */
class MaterialFactory : public AFactory<IMaterial> {
public:
    /**
     * @brief Create a material instance by name.
     *
     * @return Shared pointer to the created material, or nullptr if unknown.
     */
    std::shared_ptr<IMaterial> create(const std::string& name, const ISetting& settings) override {
        if (_factoryMap.find(name) != _factoryMap.end()) {
            IMaterial* ptr = _factoryMap[name](settings);
            return ptr ? std::shared_ptr<IMaterial>(ptr) : nullptr;
        }
        return nullptr;
    }
};
}; // namespace Raytracer
