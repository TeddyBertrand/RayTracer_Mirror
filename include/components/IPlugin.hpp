#pragma once

#include <string>

namespace Raytracer {

/**
 * @brief Base interface shared by dynamically loaded plugin objects.
 */
class IPlugin {
public:
    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     */
    virtual ~IPlugin() = default;

    /**
     * @brief Return the unique runtime name of this plugin instance/type.
     */
    virtual std::string getName() const = 0;
};

} // namespace Raytracer
