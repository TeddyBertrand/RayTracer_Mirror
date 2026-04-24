#pragma once

#include "factory/IFactory.hpp"
#include <unordered_map>

namespace Raytracer {

/**
 * @brief Shared registration storage for concrete component factories.
 *
 * @tparam T Plugin base type produced by the factory.
 */
template <typename T> class AFactory : public virtual IFactory<T> {
public:
    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     */
    virtual ~AFactory() = default;

    /**
     * @brief Create a registered component from a type name and settings.
     */
    virtual std::shared_ptr<T> create(const std::string& name,
                                      const ISetting& settings) override = 0;

    /**
     * @brief Register a constructor callback for a component type.
     *
     * @param name Type key used for runtime lookup.
     * @param init Callable creating an instance from settings.
     */
    void registerType(const std::string& name,
                      std::function<std::shared_ptr<T>(const ISetting&)> init) {
        _factoryMap[name] = init;
    }

protected:
    /** Runtime map from plugin name to constructor callback. */
    std::unordered_map<std::string, std::function<std::shared_ptr<T>(const ISetting& settings)>>
        _factoryMap;
};
}; // namespace Raytracer
