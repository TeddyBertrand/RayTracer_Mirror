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
    std::shared_ptr<ITexture> tex = nullptr;

    if (settings.exists("color")) {
        Color c = settings.getColor("color");
        tex = std::make_shared<Texture>(c);
    } else if (settings.exists("texture")) {
        std::string filename = settings.getString("texture");
        tex = std::make_shared<Texture>(filename);
    } else {
        throw std::runtime_error("LightMaterial requires either a 'color' or 'texture' setting.");
    }

    return new LightMaterial(tex);
}
}

} // namespace Raytracer
