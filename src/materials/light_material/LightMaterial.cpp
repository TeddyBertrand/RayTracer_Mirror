
#include "LightMaterial.hpp"
#include "materials/commun/texture/Texture.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "light_material"; }

IMaterial* createPlugin(const ISetting& settings) {
    std::shared_ptr<ITexture> tex = Texture::fromSetting(settings, "color");

    return new LightMaterial(tex);
}
}

} // namespace Raytracer
