#pragma once

#include "components/IPrimitive.hpp"
#include "factory/AFactory.hpp"

namespace Raytracer {

/**
 * @brief Concrete factory that creates primitive plugins from registered callbacks.
 */
class PrimitiveFactory : public AFactory<IPrimitive> {
public:
    /**
     * @brief Create a primitive instance by name.
     *
     * @return Shared pointer to the created primitive, or nullptr if unknown.
     */
    std::shared_ptr<IPrimitive> create(const std::string& name, const ISetting& settings) override {
        if (_factoryMap.find(name) != _factoryMap.end()) {
            return _factoryMap[name](settings);
        }
        return nullptr;
    }
};
}; // namespace Raytracer
