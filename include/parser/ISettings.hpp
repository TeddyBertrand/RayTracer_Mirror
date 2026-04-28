#pragma once

#include "math/Vector3D.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Raytracer {

/**
 * @brief Read-only hierarchical configuration interface.
 *
 * Implementations expose typed accessors for scene/config paths and support
 * navigation through nested groups.
 */
class ISetting {
public:
    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     */
    virtual ~ISetting() = default;

    /** @brief Read an integer value at a path. */
    virtual int getInt(const std::string& path) const = 0;

    /** @brief Read a floating-point value at a path. */
    virtual double getFloat(const std::string& path) const = 0;

    /** @brief Read a string value at a path. */
    virtual std::string getString(const std::string& path) const = 0;

    /** @brief Read a boolean value at a path. */
    virtual bool getBool(const std::string& path) const = 0;

    /** @brief Read a 3D vector value at a path. */
    virtual Vector3D getVector(const std::string& path) const = 0;

    /** @brief Read color value at a path.  */
    virtual Color getColor(const std::string& path) const = 0;

    /** @brief Check whether a path exists in the underlying configuration tree. */
    virtual bool exists(const std::string& path) const = 0;

    /**
     * @brief Return a nested configuration group rooted at `path`.
     */
    virtual std::shared_ptr<ISetting> getGroup(const std::string& path) const = 0;
};
} // namespace Raytracer
