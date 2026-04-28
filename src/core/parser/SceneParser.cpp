#include "SceneParser.hpp"
#include "parser/dlloader/DLLoaderPlugin.hpp"
#include "skies/empty_sky/EmptySky.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include <type_traits>

namespace Raytracer {

Color SceneParser::normalizeColor(const Color& color) const {
    if (color.r > 1.0 || color.g > 1.0 || color.b > 1.0) {
        return Color(color.r / 255.0, color.g / 255.0, color.b / 255.0);
    }
    return color;
}

void SceneParser::parseCamera(const libconfig::Setting& camSetting, Scene& outScene) {
    std::string type = camSetting["type"];

    LibconfigSetting cameraConfig(camSetting);
    auto camera = _factories.camera.create(type, cameraConfig);

    if (camera) {
        outScene.setCamera(std::move(camera));
    }
}

void SceneParser::parseMaterials(const libconfig::Setting& matSetting, Scene& /*outScene*/) {
    std::string type = matSetting["type"];
    std::string id = matSetting["id"];

    LibconfigSetting matConfig(matSetting);
    auto material = _factories.material.create(type, matConfig);
    if (material) {
        _materials[id] = material;
    }
}

void SceneParser::parseShapes(const libconfig::Setting& shapesSetting, Scene& outScene) {
    for (int i = 0; i < shapesSetting.getLength(); ++i) {
        const libconfig::Setting& shape = shapesSetting[i];
        std::string type = shape["type"];

        LibconfigSetting shapeConfig(shape);
        auto primitive = _factories.primitive.create(type, shapeConfig);

        if (primitive) {
            outScene.addPrimitive(std::move(primitive));
        } else {
            std::cerr << "Primitive ignoree (type inconnu): " << type << std::endl;
        }
    }
}

void SceneParser::parseLights(const libconfig::Setting& lightsSetting, Scene& outScene) {
    for (int i = 0; i < lightsSetting.getLength(); ++i) {
        const libconfig::Setting& light = lightsSetting[i];
        std::string type = light["type"];

        LibconfigSetting lightConfig(light);
        auto lightPtr = _factories.light.create(type, lightConfig);

        if (lightPtr) {
            outScene.addLight(std::move(lightPtr));
        } else {
            std::cerr << "Lumiere ignoree (type inconnu): " << type << std::endl;
        }
    }
}

void SceneParser::parseEnvironment(const libconfig::Setting& environmentSetting, Scene& outScene) {
    if (environmentSetting.exists("sky")) {
        LibconfigSetting envConfig(environmentSetting);
        const Color skyColor = normalizeColor(envConfig.getColor("sky"));
        outScene.setBackgroundColor(skyColor);
        outScene.setSky(std::make_unique<EmptySky>());
        return;
    }
    outScene.setSky(std::make_unique<EmptySky>());
}

void SceneParser::loadScene(const std::string& filePath, Scene& outScene) {
    libconfig::Config cfg;

    try {
        cfg.readFile(filePath.c_str());
        const libconfig::Setting& root = cfg.getRoot();

        _materials.clear();

        for (int i = 0; i < root.getLength(); ++i) {
            const libconfig::Setting& section = root[i];
            std::string sectionName = section.getName();

            auto it = _sectionDispatch.find(sectionName);
            if (it != _sectionDispatch.end()) {
                (this->*(it->second))(section, outScene);
            }
        }
        if (!root.exists("environment")) {
            throw SceneParserException("Scene configuration must include an 'environment' section.");
        }

    } catch (const libconfig::SettingException& e) {
        throw SceneParserException("Error parsing config file: " + std::string(e.what()));
    }
}
} // namespace Raytracer
