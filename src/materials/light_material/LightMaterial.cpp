
#include "LightMaterial.hpp"
#include "materials/commun/bsdf/normal_mapping/NormalMappingBSDF.hpp"
#include "materials/commun/texture/Texture.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "light_material"; }

IMaterial* createPlugin(const ISetting& settings) {
    std::shared_ptr<ITexture> tex = Texture::fromSetting(settings, "color");

    std::shared_ptr<IBSDF> bsdf;
    if (settings.exists("normal")) {
        auto phong_bsdf = std::make_shared<EmissiveBSDF>(tex);
        std::shared_ptr<ITexture> normal_texture = Texture::fromSetting(settings, "normal");
        double normal_strength = settings.getFloat("normal_strength", 1.0);
        bsdf = std::make_shared<NormalMappingBSDF>(phong_bsdf, normal_texture, normal_strength);
    }

    return new LightMaterial(tex);
}
}

LightMaterial::LightMaterial(std::shared_ptr<ITexture> tex,
        std::shared_ptr<IBSDF> custom_bsdf) {
    if (custom_bsdf) {
        _bsdf = custom_bsdf;
    } else {
        _bsdf = std::make_shared<EmissiveBSDF>(tex);
    }
}

} // namespace Raytracer
