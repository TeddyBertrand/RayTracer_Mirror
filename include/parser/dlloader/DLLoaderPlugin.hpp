#pragma once

#include "factory/LightFactory.hpp"
#include "factory/MaterialFactory.hpp"
#include "factory/PrimitiveFactory.hpp"
#include "parser/dlloader/DLLoader.hpp"
#include <functional>

namespace Raytracer {

/**
 * @brief Specialization layer exposing typed registration entry-points.
 *
 * This wrapper keeps symbol retrieval type-safe for each plugin category.
 */
class DLLoaderPlugin : public DLLoader {
public:
    /**
     * @brief Signature of primitive registration functions exported by plugins.
     */
    using primitiveFunction = void (*)(PrimitiveFactory&);

    /**
     * @brief Retrieve primitive registration function from a library.
     */
    primitiveFunction getPrimitiveFunction(const std::string& registerFunction,
                                           const std::string& libName) const {
        return DLLoader::getFunction<primitiveFunction>(registerFunction, libName);
    }

    /**
     * @brief Signature of material registration functions exported by plugins.
     */
    using materialFunction = void (*)(MaterialFactory&);

    /**
     * @brief Retrieve material registration function from a library.
     */
    materialFunction getMaterialFunction(const std::string& registerFunction,
                                         const std::string& libName) const {
        return DLLoader::getFunction<materialFunction>(registerFunction, libName);
    }

    /**
     * @brief Signature of light registration functions exported by plugins.
     */
    using lightFunction = void (*)(MaterialFactory&);

    /**
     * @brief Retrieve light registration function from a library.
     */
    lightFunction getLightFunction(const std::string& registerFunction,
                                   const std::string& libName) const {
        return DLLoader::getFunction<lightFunction>(registerFunction, libName);
    }
};
}; // namespace Raytracer
