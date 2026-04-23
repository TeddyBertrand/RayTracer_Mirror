#pragma once

#include "factory/LightFactory.hpp"
#include "factory/MaterialFactory.hpp"
#include "factory/PrimitiveFactory.hpp"
#include "parser/dlloader/DLLoader.hpp"
#include <functional>

namespace Raytracer {
class DLLoaderPlugin : public DLLoader {
public:
    /**
     * @brief encapsulate DLLoader getFunction for Primitive factory
     */
    using primitiveFunction = void (*)(PrimitiveFactory&);

    primitiveFunction getPrimitiveFunction(const std::string& registerFunction,
                                           const std::string& libName) const {
        return DLLoader::getFunction<primitiveFunction>(registerFunction, libName);
    }

    /**
     * @brief encapsulate DLLoader getFunction for Material factory
     */
    using materialFunction = void (*)(MaterialFactory&);
    materialFunction getMaterialFunction(const std::string& registerFunction,
                                         const std::string& libName) const {
        return DLLoader::getFunction<materialFunction>(registerFunction, libName);
    }

    /**
     * @brief encapsulate DLLoader getFunction for Light factory
     */
    using lightFunction = void (*)(MaterialFactory&);
    lightFunction getLightFunction(const std::string& registerFunction,
                                   const std::string& libName) const {
        return DLLoader::getFunction<lightFunction>(registerFunction, libName);
    }
};
}; // namespace Raytracer
