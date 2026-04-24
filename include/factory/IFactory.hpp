#pragma once

#include "components/IPlugin.hpp"
#include "parser/ISettings.hpp"
#include <functional>
#include <memory>
#include <string>

namespace Raytracer {

/**
 * @brief Generic factory interface used to instantiate plugin components by name.
 *
 * @tparam T Plugin base type produced by the factory.
 */
template <typename T> class IFactory {
public:
    /**
     * @brief Create a component instance from its registered type name.
     *
     * @param name Registered component identifier.
     * @param settings Configuration group used for construction.
     * @return Shared pointer to created instance, or nullptr when unsupported.
     */
    virtual std::shared_ptr<T> create(const std::string& name, const ISetting& settings) = 0;
};
}; // namespace Raytracer
