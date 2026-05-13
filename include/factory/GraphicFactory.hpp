#pragma once

#include "components/IGraphic.hpp"
#include "factory/AFactory.hpp"

namespace Raytracer {

class GraphicFactory : public AFactory<IGraphic> {
public:
    std::shared_ptr<IGraphic> create(const std::string& name, const ISetting& settings) override {
        if (_factoryMap.find(name) != _factoryMap.end()) {
            IGraphic* ptr = _factoryMap[name](settings);
            return ptr ? std::shared_ptr<IGraphic>(ptr) : nullptr;
        }
        return nullptr;
    }
};

} // namespace Raytracer
