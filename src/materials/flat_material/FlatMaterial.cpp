#include "FlatMaterial.hpp"
#include "materials/commun/texture/Texture.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

std::unique_ptr<IBSDF> FlatMaterial::getBSDF(const HitRecord& hit) const {
    Color color = _albedo->value(hit.u, hit.v);

    return std::make_unique<LambertianBSDF>(color, _randomness);
}

extern "C" {

const char* getName() { return "flat_color"; }

IMaterial* createPlugin(const ISetting& settings) {
    double randomness = settings.getFloat("randomness", 0.0);
    std::shared_ptr<ITexture> tex = Texture::fromSetting(settings, "color");

    return new FlatMaterial(tex, randomness);
}
}

} // namespace Raytracer
