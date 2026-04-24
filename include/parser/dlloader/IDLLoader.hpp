#pragma once
#include <string>

namespace Raytracer {

/**
 * @brief Abstract interface for runtime shared-library loading.
 */
class IDLLoader {
public:
    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     */
    virtual ~IDLLoader() = default;

    /**
     * @brief Open and register a shared library from its filesystem path.
     * @param libraryPath Absolute or relative path to a `.so` file.
     */
    virtual void openLibrary(const std::string& libraryPath) = 0;

    /**
     * @brief Close all currently loaded libraries and release handles.
     */
    virtual void closeLibrary() = 0;
};
} // namespace Raytracer
