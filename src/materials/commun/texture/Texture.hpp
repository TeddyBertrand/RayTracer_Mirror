#pragma once

#include "components/ITexture.hpp"
#include "materials/commun/texture/ImageTexture.hpp"
#include "materials/commun/texture/PerlinTexture.hpp"
#include "materials/commun/texture/SolidColor.hpp"
#include "parser/ISettings.hpp"
#include <exception>
#include <functional>
#include <memory>
#include <unordered_map>

namespace Raytracer {

/**
 * @brief Factory wrapper to create different types of textures.
 * Allows easy extensibility for new texture types.
 */
class Texture {
public:
    using TexturePtr = std::shared_ptr<ITexture>;
    using TextureFactory = std::function<TexturePtr(const ISetting&, const std::string&)>;
    using ProceduralTextureFactory = std::function<TexturePtr(const std::shared_ptr<ISetting>&)>;

    static TexturePtr fromSetting(const ISetting& settings, const std::string& key) {
        if (!settings.exists(key)) {
            throw std::runtime_error("Missing texture setting '" + key + "'.");
        }

        if (auto texture = tryCreateColor(settings, key))
            return texture;
        if (auto texture = tryCreateImage(settings, key))
            return texture;
        if (auto texture = tryCreateProcedural(settings, key))
            return texture;

        throw std::runtime_error("Unknown texture format for key: " + key);
    }

private:
    static const std::unordered_map<std::string, ProceduralTextureFactory>&
    getProceduralFactories() {
        static const std::unordered_map<std::string, ProceduralTextureFactory> factories = {
            {"perlin",
             [](const std::shared_ptr<ISetting>& group) {
                 double scale = group->exists("scale") ? group->getFloat("scale") : 1.0;
                 Color a = group->getColor("color_a");
                 Color b = group->getColor("color_b");
                 return std::make_shared<PerlinTexture>(scale, a, b);
             }},
        };
        return factories;
    }

    static TexturePtr tryCreateColor(const ISetting& settings, const std::string& key) {
        try {
            return std::make_shared<SolidColor>(settings.getColor(key));
        } catch (...) {
            return nullptr;
        }
    }

    static TexturePtr tryCreateImage(const ISetting& settings, const std::string& key) {
        try {
            return std::make_shared<ImageTexture>(settings.getString(key));
        } catch (...) {
            return nullptr;
        }
    }

    static TexturePtr tryCreateProcedural(const ISetting& settings, const std::string& key) {
        try {
            auto group = settings.getGroup(key);
            std::string type = group->getString("type");

            const auto& factories = getProceduralFactories();
            auto it = factories.find(type);

            if (it == factories.end())
                return nullptr;

            return it->second(group);
        } catch (...) {
            return nullptr;
        }
    }
};

} // namespace Raytracer
