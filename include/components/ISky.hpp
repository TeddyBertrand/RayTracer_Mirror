#pragma once

#include "math/Ray.hpp"
#include "math/Color.hpp"

#include "components/IPlugin.hpp"

namespace Raytracer
{

class ISky : public IPlugin
{
public:
    virtual ~ISky() = default;

    /*
        @brief Get the color of the background in the direction of the ray.
        @param r The ray for which to get the background color.
    */
    virtual Color getBackgroundColor(const Ray& r) const = 0;

    /*
        @brief Get the color of the environment in the direction of the ray.
        @param r The ray for which to get the environment color.
    */
    virtual Color getEnvironmentColor(const Ray& r) const = 0;
};

} // namespace Raytracer
