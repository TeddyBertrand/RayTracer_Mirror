#include "FlatMaterial.hpp"
#include "parser/ISettings.hpp"
#include "materials/commun/texture/Texture.hpp"

namespace Raytracer {

std::unique_ptr<IBSDF> FlatMaterial::getBSDF(const HitRecord& hit) const {
    Color color = _albedo->value(hit.u, hit.v);

    return std::make_unique<LambertianBSDF>(color);
}

extern "C" {

const char* getName() { return "flat_color"; }

IMaterial* createPlugin(const ISetting& settings) {
    std::shared_ptr<ITexture> tex = nullptr;

    if (settings.exists("color")) {
        Color c = settings.getColor("color");
        tex = std::make_shared<Texture>(c);
    } else if (settings.exists("texture")) {
        std::string filename = settings.getString("texture");
        tex = std::make_shared<Texture>(filename);
    } else {
        throw std::runtime_error("FlatMaterial requires either a 'color' or 'texture' setting.");
    }

    return new FlatMaterial(tex);
}
}

} // namespace Raytracer
