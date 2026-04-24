#pragma once

#include "math/Color.hpp"
#include "math/Ray.hpp"

#include "components/IPlugin.hpp"

namespace Raytracer {

/**
 * @brief Interface for procedural/environment sky models.
 *
 * Sky implementations provide background radiance for rays that miss scene
 * geometry and may also expose environment lighting lookups.
 */
class ISky : public IPlugin {
public:
    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     */
    virtual ~ISky() = default;

    /**
     * @brief Evaluate background color for a camera ray that did not hit geometry.
     *
     * @param r Query ray direction.
     * @return Background radiance color.
     */
    virtual Color getBackgroundColor(const Ray& r) const = 0;

    /**
     * @brief Evaluate environment lighting for an arbitrary direction.
     *
     * @param r Query ray direction.
     * @return Environment radiance color.
     */
    virtual Color getEnvironmentColor(const Ray& r) const = 0;
};

} // namespace Raytracer
