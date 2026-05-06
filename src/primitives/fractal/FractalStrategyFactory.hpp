#pragma once

#include "parser/ISettings.hpp"
#include "strategy/IFractalStrategy.hpp"
#include <string>
#include <unordered_map>

namespace Raytracer {

class FractalStrategyFactory {
public:
    static std::unique_ptr<IFractalStrategy> create(std::string type, const ISetting& settings);
};

} // namespace Raytracer
