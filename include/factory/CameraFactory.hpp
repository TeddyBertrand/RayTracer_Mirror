#pragma once

#include "components/ICamera.hpp"
#include "factory/AFactory.hpp"

namespace Raytracer {
class CameraFactory : public AFactory<ICamera> {
public:
    std::shared_ptr<ICamera> create(const std::string& name, const ISetting& settings) override {
        if (_factoryMap.find(name) != _factoryMap.end()) {
            ICamera* ptr = _factoryMap[name](settings);
            return ptr ? std::shared_ptr<ICamera>(ptr) : nullptr;
        }
        return nullptr;
    }
};
}; // namespace Raytracer
