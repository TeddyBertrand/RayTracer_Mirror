#pragma once

#include "components/IPrimitive.hpp"
#include "factory/AFactory.hpp"

namespace Raytracer
{
    class PrimitiveFactory : public AFactory<IPrimitive>
    {
        public:
            std::shared_ptr<IPrimitive> create(const std::string &name, const ISetting& settings) override
            {
                if (_factoryMap.find(name) != _factoryMap.end()) {
                    return _factoryMap[name](settings);
                }
                return nullptr;
            }
    };
};
