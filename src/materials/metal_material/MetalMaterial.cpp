
#include "MetalMaterial.hpp"
#include "materials/commun/bsdf/normal_mapping/NormalMappingBSDF.hpp"
#include "materials/commun/texture/Texture.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "metal"; }

IMaterial* createPlugin(const ISetting& settings) {
    double fuzz = settings.getFloat("fuzz", 0.0);
    std::shared_ptr<ITexture> tex = Texture::fromSetting(settings, "color");

    std::shared_ptr<IBSDF> bsdf;
    if (settings.exists("normal")) {
        auto phong_bsdf = std::make_shared<MetalBSDF>(tex, fuzz);
        std::shared_ptr<ITexture> normal_texture = Texture::fromSetting(settings, "normal");
        double normal_strength = settings.getFloat("normal_strength", 1.0);
        bsdf = std::make_shared<NormalMappingBSDF>(phong_bsdf, normal_texture, normal_strength);
    }

    return new MetalMaterial(tex, fuzz, bsdf);
}
}

MetalMaterial::MetalMaterial(std::shared_ptr<ITexture> tex,
                double fuzz, std::shared_ptr<IBSDF> custom_bsdf)
    : _fuzz(fuzz < 0.0 ? 0.0 : (fuzz > 1.0 ? 1.0 : fuzz)) {
    if (custom_bsdf) {
        _bsdf = custom_bsdf;
    } else {
        _bsdf = std::make_shared<MetalBSDF>(tex, _fuzz);
    }
}

} // namespace Raytracer
