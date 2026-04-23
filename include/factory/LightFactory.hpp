#pragma once

#include "components/ILight.hpp"
#include "factory/AFactory.hpp"

namespace Raytracer {
class LightFactory : public AFactory<ILight> {
public:
    std::shared_ptr<ILight> create(const std::string& name, const ISetting& settings) override {
        if (_factoryMap.find(name) != _factoryMap.end()) {
            return _factoryMap[name](settings);
        }
        return nullptr;
    }
};
}; // namespace Raytracer
