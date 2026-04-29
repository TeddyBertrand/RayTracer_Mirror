#pragma once

#include "math/Color.hpp"
#include "math/Vector3D.hpp"
#include <memory>
#include <string>

namespace Raytracer {

/**
 * @class ISetting
 * @brief Read-only interface for scene configuration.
 * * This interface provides a type-safe and secure way to access data from scene configuration
 * files (such as .libconfig, JSON, or YAML) using dot-separated paths (e.g.,
 * "camera.resolution.width"). It supports basic primitives, geometric types (Vector3D, Color), and
 * nested groups.
 */
class ISetting {
public:
    virtual ~ISetting() = default;

    /**
     * @brief Checks if a specific setting or group exists at the given path.
     * @param path The lookup path in the configuration (e.g., "scene.ambient").
     * @return true if the path exists, false otherwise.
     */
    virtual bool exists(const std::string& path) const = 0;

    /**
     * @brief Retrieves an integer value.
     * @param path The lookup path.
     * @return The integer value found at the path.
     * @throw std::exception (or implementation-specific error) if the path is missing or the type
     * is incorrect.
     */
    virtual int getInt(const std::string& path) const = 0;

    /**
     * @brief Retrieves an integer value with a fallback default.
     * @param path The lookup path.
     * @param defaultValue The value to return if the path does not exist.
     * @return The value at the path if it exists, otherwise defaultValue.
     */
    virtual int getInt(const std::string& path, int defaultValue) const {
        return exists(path) ? getInt(path) : defaultValue;
    }

    /**
     * @brief Retrieves a floating-point (double) value.
     * @param path The lookup path.
     * @return The double value found at the path.
     * @throw std::exception if the path is missing or the value cannot be converted to double.
     */
    virtual double getFloat(const std::string& path) const = 0;

    /**
     * @brief Retrieves a double value with a fallback default.
     * @param path The lookup path.
     * @param defaultValue The value to return if the path does not exist.
     * @return The value at the path if it exists, otherwise defaultValue.
     */
    virtual double getFloat(const std::string& path, double defaultValue) const {
        return exists(path) ? getFloat(path) : defaultValue;
    }

    /**
     * @brief Retrieves a string value.
     * @param path The lookup path.
     * @return The string found at the path.
     * @throw std::exception if the path is missing.
     */
    virtual std::string getString(const std::string& path) const = 0;

    /**
     * @brief Retrieves a string value with a fallback default.
     * @param path The lookup path.
     * @param defaultValue The string to return if the path does not exist.
     * @return The value at the path if it exists, otherwise defaultValue.
     */
    virtual std::string getString(const std::string& path, const std::string& defaultValue) const {
        return exists(path) ? getString(path) : defaultValue;
    }

    /**
     * @brief Overload for retrieving a string with a C-style string default.
     * @param path The lookup path.
     * @param defaultValue The C-string to return if the path does not exist.
     * @return The value at the path or a string constructed from defaultValue.
     */
    virtual std::string getString(const std::string& path, const char* defaultValue) const {
        return exists(path) ? getString(path) : std::string(defaultValue);
    }

    /**
     * @brief Retrieves a boolean value.
     * @param path The lookup path.
     * @return The boolean value found at the path.
     * @throw std::exception if the path is missing or not a boolean.
     */
    virtual bool getBool(const std::string& path) const = 0;

    /**
     * @brief Retrieves a boolean value with a fallback default.
     * @param path The lookup path.
     * @param defaultValue The value to return if the path does not exist.
     * @return The value at the path if it exists, otherwise defaultValue.
     */
    virtual bool getBool(const std::string& path, bool defaultValue) const {
        return exists(path) ? getBool(path) : defaultValue;
    }

    /**
     * @brief Retrieves a 3D vector.
     * Expected format in config is usually a group or array: {x=1.0; y=2.0; z=3.0}.
     * @param path The lookup path.
     * @return A Vector3D object containing the coordinates.
     * @throw std::exception if the path is missing or coordinates are invalid.
     */
    virtual Vector3D getVector(const std::string& path) const = 0;

    /**
     * @brief Retrieves a 3D vector with a fallback default.
     * @param path The lookup path.
     * @param defaultValue The Vector3D to return if the path does not exist.
     * @return The Vector3D at the path if it exists, otherwise defaultValue.
     */
    virtual Vector3D getVector(const std::string& path, const Vector3D& defaultValue) const {
        return exists(path) ? getVector(path) : defaultValue;
    }

    /**
     * @brief Retrieves a Color value.
     * Expected format in config is usually: {r=255; g=255; b=255}.
     * @param path The lookup path.
     * @return A Color object representing the RGB values.
     * @throw std::exception if the path is missing or the color format is invalid.
     */
    virtual Color getColor(const std::string& path) const = 0;

    /**
     * @brief Retrieves a Color value with a fallback default.
     * @param path The lookup path.
     * @param defaultValue The Color to return if the path does not exist.
     * @return The Color at the path if it exists, otherwise defaultValue.
     */
    virtual Color getColor(const std::string& path, const Color& defaultValue) const {
        return exists(path) ? getColor(path) : defaultValue;
    }

    /**
     * @brief Navigates into a nested configuration group.
     * Useful for isolating parameters for a specific component (e.g., a single primitive).
     * @param path The lookup path to the group (e.g., "camera.transform").
     * @return A shared pointer to a new ISetting instance representing the sub-group.
     * @throw std::exception if the group does not exist.
     */
    virtual std::shared_ptr<ISetting> getGroup(const std::string& path) const = 0;
};

} // namespace Raytracer
