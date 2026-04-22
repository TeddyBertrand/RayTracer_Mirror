#pragma once

#include <functional>
#include "parser/dlloader/DLLoader.hpp"
#include "factory/PrimitiveFactory.hpp"
#include "factory/MaterialFactory.hpp"
#include "factory/LightFactory.hpp"

namespace Raytracer
{
    class DLLoaderPlugin : public DLLoader
    {
        public:
            using primitiveFunction = void(*)(PrimitiveFactory &);
            primitiveFunction getPrimitiveFunction(const std::string &entryPoint) const
            {
                return DLLoader::getFunction<primitiveFunction>(entryPoint);
            }

            using materialFunction = void(*)(MaterialFactory &);
            materialFunction getMaterialFunction(const std::string &entryPoint) const
            {
                return DLLoader::getFunction<materialFunction>(entryPoint);
            }

            using lightFunction = void(*)(MaterialFactory &);
            lightFunction getLightFunction(const std::string &entryPoint) const
            {
                return DLLoader::getFunction<lightFunction>(entryPoint);
            }
    };
};
