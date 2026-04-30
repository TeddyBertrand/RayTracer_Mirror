#include "LightMaterial.hpp"
#include "materials/commun/texture/Texture.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

std::unique_ptr<IBSDF> LightMaterial::getBSDF(const HitRecord& rec) const {
    Color c = _emission_tex->value(rec.u, rec.v);
    return std::make_unique<EmissiveBSDF>(c);
}

extern "C" {

const char* getName() { return "light_material"; }

IMaterial* createPlugin(const ISetting& settings) {
    std::shared_ptr<ITexture> tex = Texture::fromSetting(settings, "color");

    return new LightMaterial(tex);
}
}

} // namespace Raytracer
