#include "SceneParser.hpp"
#include "parser/dlloader/DLLoaderPlugin.hpp"
#include "skies/empty_sky/EmptySky.hpp"
#include <filesystem>
#include <iostream>
#include <string>

namespace Raytracer {

void SceneParser::parseCamera(const libconfig::Setting& camSetting, Scene& outScene) {
    std::string type = camSetting["type"];

    LibconfigSetting cameraConfig(camSetting);
    auto camera = _factories.camera.create(type, cameraConfig);

    if (camera) {
        outScene.setCamera(std::move(camera));
    }
}

void SceneParser::parseMaterials(const libconfig::Setting& matsSetting, Scene& outScene) {
    for (int i = 0; i < matsSetting.getLength(); ++i) {
        const libconfig::Setting& mat = matsSetting[i];

        std::string type = mat["type"];

        std::string id = mat["id"];

        LibconfigSetting matConfig(mat);

        auto material = _factories.material.create(type, matConfig);

        if (material) {
            outScene.addMaterial(id, material);
        }
    }
}

void SceneParser::parseShapes(const libconfig::Setting& shapesSetting, Scene& outScene) {
    for (int i = 0; i < shapesSetting.getLength(); ++i) {
        LibconfigSetting baseConfig(shapesSetting[i]);

        PrimitiveSetting shapeConfig(baseConfig, outScene.getMaterials());

        if (shapeConfig.exists("material") && !shapeConfig.getMaterial()) {
            std::cerr << "Forme ignoree: materiau introuvable ("
                      << shapeConfig.getString("material") << ")" << std::endl;
            continue;
        }

        std::string type = shapesSetting[i]["type"];
        auto primitive = _factories.primitive.create(type, shapeConfig);

        if (primitive) {
            outScene.addPrimitive(std::move(primitive));
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

void SceneParser::parseSky(const libconfig::Setting& skySetting, Scene& outScene) {
    std::string type = skySetting["type"];
    LibconfigSetting skyConfig(skySetting);

    auto sky = _factories.sky.create(type, skyConfig);
    if (sky) {
        outScene.setSky(std::move(sky));
    } else {
        outScene.setSky(std::make_unique<EmptySky>());
    }
    return;
}

void SceneParser::loadScene(const std::string& filePath, Scene& outScene) {
    libconfig::Config cfg;

    try {
        cfg.readFile(filePath.c_str());
        const libconfig::Setting& root = cfg.getRoot();

        if (root.exists("materials")) {
            const libconfig::Setting& materials = root["materials"];
            auto it = _sectionDispatch.find("materials");
            if (it != _sectionDispatch.end()) {
                (this->*(it->second))(materials, outScene);
            }
        }

        for (int i = 0; i < root.getLength(); ++i) {
            const libconfig::Setting& section = root[i];
            std::string sectionName = section.getName();

            if (sectionName == "materials")
                continue;

            auto it = _sectionDispatch.find(sectionName);
            if (it != _sectionDispatch.end()) {
                (this->*(it->second))(section, outScene);
            }
        }

    } catch (const libconfig::SettingException& e) {
        throw SceneParserException("Error parsing config file: " + std::string(e.what()));
    }
}
} // namespace Raytracer
