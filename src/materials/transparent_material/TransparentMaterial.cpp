#include "TransparentMaterial.hpp"
#include "materials/commun/texture/Texture.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "transparent"; }

IMaterial* createPlugin(const ISetting& settings) {
    double ref = settings.getFloat("ref", 0.0);
    std::shared_ptr<ITexture> tex = Texture::fromSetting(settings, "color");

    return new TransparentMaterial(tex, ref);
}
}

} // namespace Raytracer
