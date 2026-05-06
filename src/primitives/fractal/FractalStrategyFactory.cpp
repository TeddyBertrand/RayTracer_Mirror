#include "FractalStrategyFactory.hpp"
#include <functional>

#include "strategy/mandelbulb/MandelbulbStrategy.hpp"
#include "strategy/menger_sponge/MengerSpongeStrategy.hpp"

namespace Raytracer {

std::unique_ptr<IFractalStrategy> FractalStrategyFactory::create(std::string type,
                                                                 const ISetting& settings) {

    using StrategyCreator = std::function<std::unique_ptr<IFractalStrategy>(const ISetting&)>;

    static const std::unordered_map<std::string, StrategyCreator> strategyMap = {
        {"menger_sponge",
         []([[maybe_unused]] const ISetting& settings) { return std::make_unique<MengerSpongeStrategy>(); }},
        {"mandelbulb",
         []([[maybe_unused]] const ISetting& settings) { return std::make_unique<MandelbulbStrategy>(); }},
    };

    auto it = strategyMap.find(type);
    if (it != strategyMap.end()) {
        return it->second(settings);
    } else {
        throw std::runtime_error("Unknown fractal type: " + type);
    }
}

} // namespace Raytracer
