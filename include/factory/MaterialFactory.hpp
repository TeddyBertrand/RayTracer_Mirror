#pragma once

#include "components/IMaterial.hpp"
#include "factory/AFactory.hpp"

namespace Raytracer
{
    class MaterialFactory : public AFactory<IMaterial>
    {
        public:
            std::shared_ptr<IMaterial> create(const std::string &name, const ISetting& settings) override
            {
                if (_factoryMap.find(name) != _factoryMap.end()) {
                    return _factoryMap[name](settings);
                }
                return nullptr;
            }
    };
};
