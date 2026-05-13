
#include "FlatMaterial.hpp"
#include "materials/commun/bsdf/normal_mapping/NormalMappingBSDF.hpp"
#include "materials/commun/texture/Texture.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "flat_color"; }

IMaterial* createPlugin(const ISetting& settings) {
    double randomness = settings.getFloat("randomness", 0.0);
    std::shared_ptr<ITexture> tex = Texture::fromSetting(settings, "color");

    std::shared_ptr<IBSDF> bsdf;
    if (settings.exists("normal")) {
        auto phong_bsdf = std::make_shared<LambertianBSDF>(tex, randomness);
        std::shared_ptr<ITexture> normal_texture = Texture::fromSetting(settings, "normal");
        double normal_strength = settings.getFloat("normal_strength", 1.0);
        bsdf = std::make_shared<NormalMappingBSDF>(phong_bsdf, normal_texture, normal_strength);
    }

    return new FlatMaterial(tex, randomness, bsdf);
}
}

FlatMaterial::FlatMaterial(std::shared_ptr<ITexture> tex,
        double randomness,
        std::shared_ptr<IBSDF> custom_bsdf)
    : _randomness(randomness < 0.0 ? 0.0 : (randomness > 1.0 ? 1.0 : randomness)) {
    if (custom_bsdf) {
        _bsdf = custom_bsdf;
    } else {
        _bsdf = std::make_shared<LambertianBSDF>(tex, _randomness);
    }
}

} // namespace Raytracer
