#pragma once

#include "components/IRenderer.hpp"
#include "factory/AFactory.hpp"

namespace Raytracer {
class RendererFactory : public AFactory<IRenderer> {
public:
    std::shared_ptr<IRenderer> create(const std::string& name, const ISetting& settings) override {
        if (_factoryMap.find(name) != _factoryMap.end()) {
            IRenderer* ptr = _factoryMap[name](settings);
            return ptr ? std::shared_ptr<IRenderer>(ptr) : nullptr;
        }
        return nullptr;
    }
};
} // namespace Raytracer
