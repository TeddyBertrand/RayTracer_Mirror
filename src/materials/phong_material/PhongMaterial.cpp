
#include "PhongMaterial.hpp"
#include "materials/commun/bsdf/normal_mapping/NormalMappingBSDF.hpp"
#include "materials/commun/texture/Texture.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "phong"; }

IMaterial* createPlugin(const ISetting& settings) {
    double spec = settings.getFloat("spec", 0.0);
    std::shared_ptr<ITexture> tex = Texture::fromSetting(settings, "color");

    std::shared_ptr<IBSDF> bsdf;
    if (settings.exists("normal")) {
        auto phong_bsdf = std::make_shared<PhongBSDF>(tex, spec);
        std::shared_ptr<ITexture> normal_texture = Texture::fromSetting(settings, "normal");
        double normal_strength = settings.getFloat("normal_strength", 1.0);
        bsdf = std::make_shared<NormalMappingBSDF>(phong_bsdf, normal_texture, normal_strength);
    }

    return new PhongMaterial(tex, spec, bsdf);
}
}

} // namespace Raytracer
