
#include "MetalMaterial.hpp"
#include "materials/commun/texture/Texture.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "metal"; }

IMaterial* createPlugin(const ISetting& settings) {
    double fuzz = settings.getFloat("fuzz", 0.0);
    std::shared_ptr<ITexture> tex = Texture::fromSetting(settings, "color");

    return new MetalMaterial(tex, fuzz);
}
}

} // namespace Raytracer
