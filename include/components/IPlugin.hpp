#pragma once

#include <string>

namespace Raytracer {

class IPlugin {
public:
    virtual ~IPlugin() = default;

    virtual std::string getName() const = 0;
};

} // namespace Raytracer
