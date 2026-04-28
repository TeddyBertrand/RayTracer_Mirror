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

void SceneParser::loadFactoryPlugins() {
    const std::string pluginsPath = "./plugins";

    if (!std::filesystem::exists(pluginsPath)) {
        std::cerr << "[Parser] Erreur : Le dossier ./plugins est introuvable." << std::endl;
        return;
    }

    DLLoaderPlugin loader;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(pluginsPath)) {

        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".dll" || ext == ".so") {

                std::string path = entry.path().string();

                if (auto func = loader.getCameraFunction("registerPlugin", path)) {
                    func(_factories.camera);
                    continue;
                }

                if (auto func = loader.getPrimitiveFunction("registerPlugin", path)) {
                    func(_factories.primitive);
                    continue;
                }

                if (auto func = loader.getLightFunction("registerPlugin", path)) {
                    func(_factories.light);
                    continue;
                }

                if (auto func = loader.getMaterialFunction("registerPlugin", path)) {
                    func(_factories.material);
                    continue;
                }
            }
        }
    }
}

void SceneParser::loadScene(const std::string& filePath, Scene& outScene) {
    libconfig::Config cfg;

    try {
        loadFactoryPlugins();
        cfg.readFile(filePath.c_str());
        const libconfig::Setting& root = cfg.getRoot();

        _materials.clear();

        if (root.exists("environment")) {
            parseEnvironment(root["environment"], outScene);
        } else {
            outScene.setSky(std::make_unique<EmptySky>());
        }

        if (root.exists("camera")) {
            parseCamera(root["camera"], outScene);
        }

        if (root.exists("materials")) {
            const libconfig::Setting& mats = root["materials"];
            for (int i = 0; i < mats.getLength(); ++i) {
                parseMaterials(mats[i], outScene);
            }
        }

        if (root.exists("shapes")) {
            parseShapes(root["shapes"], outScene);
        }

        if (root.exists("lights")) {
            const libconfig::Setting& lightsVisual = root["lights"];
            if (lightsVisual.exists("list")) {
                parseLights(lightsVisual["list"], outScene);
            }
        }
    } catch (const libconfig::FileIOException&) {
        std::cerr << "Erreur : Impossible de lire le fichier " << filePath << std::endl;
    } catch (const libconfig::ParseException& pex) {
        std::cerr << "Erreur de syntaxe dans " << pex.getFile() << " a la ligne " << pex.getLine()
                  << " : " << pex.getError() << std::endl;
    } catch (const libconfig::SettingNotFoundException& nfex) {
        std::cerr << "Champ manquant dans le fichier de configuration : " << nfex.getPath()
                  << std::endl;
    } catch (const libconfig::SettingTypeException& stex) {
        std::cerr << "Mauvais type pour le champ : " << stex.getPath() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Erreur inattendue : " << ex.what() << std::endl;
    }
}
} // namespace Raytracer
