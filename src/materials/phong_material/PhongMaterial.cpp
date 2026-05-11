
#include "PhongMaterial.hpp"
#include "materials/commun/texture/Texture.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "phong"; }

IMaterial* createPlugin(const ISetting& settings) {
    double spec = settings.getFloat("spec", 0.0);
    std::shared_ptr<ITexture> tex = Texture::fromSetting(settings, "color");

    return new PhongMaterial(tex, spec);
}
}

} // namespace Raytracer
