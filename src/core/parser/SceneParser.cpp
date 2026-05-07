#include "SceneParser.hpp"
#include "builder/EntityBuilder.hpp"
#include "components/Entity.hpp"
#include "math/MathUtils.hpp"
#include "parser/dlloader/DLLoaderPlugin.hpp"
#include "skies/empty_sky/EmptySky.hpp"
#include <filesystem>
#include <iostream>
#include <string>

namespace Raytracer {

void SceneParser::loadScene(const std::string& filePath, Scene& outScene) {
    libconfig::Config cfg;

    try {
        _manager.trackFile(filePath);
        cfg.readFile(filePath.c_str());
        const libconfig::Setting& root = cfg.getRoot();

        if (root.exists("materials")) {
            parseMaterials(root["materials"], outScene);
        }

        for (int i = 0; i < root.getLength(); ++i) {
            const libconfig::Setting& section = root[i];
            std::string name = section.getName();

            auto it = _sectionDispatch.find(name);
            if (it != _sectionDispatch.end()) {
                (this->*(it->second))(section, outScene);
            }
        }

        _manager.untrackFile(filePath);
    } catch (const std::exception& e) {
        _manager.untrackFile(filePath);
        throw SceneParserException(std::string("Error loading ") + filePath + ": " + e.what());
    }
}

void SceneParser::parseShapes(const libconfig::Setting& setting, Scene& outScene) {
    for (int i = 0; i < setting.getLength(); ++i) {
        const libconfig::Setting& shapeSetting = setting[i];

        if (shapeSetting.exists("path")) {
            handleImport(shapeSetting, outScene);
        } else {
            auto primitive = handleStandardPrimitive(shapeSetting, outScene);
            if (primitive) {
                outScene.addPrimitive(std::move(primitive));
            }
        }
    }
}

std::shared_ptr<IPrimitive> SceneParser::handleImport(const libconfig::Setting& setting,
                                                      Scene& outScene) {
    std::string path = setting["path"];
    std::string name = setting.exists("name") ? (const char*)setting["name"] : "sub";

    _manager.pushNamespace(name);
    _manager.pushTransformation(parseMatrix(setting));

    loadScene(path, outScene, nullptr);

    _manager.popTransformation();
    _manager.popNamespace();

    return nullptr;
}

std::shared_ptr<IPrimitive> SceneParser::handleStandardPrimitive(const libconfig::Setting& setting,
                                                                 Scene& outScene) {
    LibconfigSetting baseConfig(setting);
    PrimitiveSetting shapeConfig(baseConfig,
                                 _manager.getContextualMaterials(outScene.getMaterials()));

    auto primitivePtr = _factories.primitive.create(setting["type"], shapeConfig);
    if (!primitivePtr)
        return nullptr;

    // On applique la transformation cumulative du manager
    auto entity = std::dynamic_pointer_cast<Entity>(primitivePtr);
    if (entity) {
        entity->setTransform(_manager.getCurrentTransformation() * entity->getTransform());
    }

    return primitivePtr;
}

void SceneParser::parseCamera(const libconfig::Setting& setting, Scene& outScene) {
    LibconfigSetting cameraConfig(setting);
    if (!cameraConfig.exists("type"))
        return;
    auto camera = _factories.camera.create(cameraConfig.getString("type"), cameraConfig);
    if (camera)
        outScene.setCamera(std::move(camera));
}

void SceneParser::parseRender(const libconfig::Setting& renderSetting, Scene& outScene) {
    (void)outScene;
    if (!renderSetting.exists("samples"))
        return;

    try {
        const int s = static_cast<int>(renderSetting["samples"]);
        if (s < 1) {
            throw RenderSettingsException("render.samples must be >= 1 (received " +
                                          std::to_string(s) + ")");
        } else if (s > 100000) {
            std::cerr << "Warning: render.samples too large, clamping to 100000" << std::endl;
            _renderSamples = 100000;
        } else {
            _renderSamples = s;
        }
    } catch (const libconfig::SettingTypeException&) {
        throw RenderSettingsException("render.samples has an invalid type (expected integer)");
    } catch (const libconfig::SettingNotFoundException&) {
        throw RenderSettingsException("render.samples is missing");
    }

    if (renderSetting.exists("adaptive_threshold")) {
        try {
            _renderThreshold = static_cast<double>(renderSetting["adaptive_threshold"]);
            if (_renderThreshold < 0.0 || _renderThreshold > 1.0) {
                std::cerr << "Warning: render.adaptive_threshold must be between 0.0 and 1.0, "
                          << "using default 0.1" << std::endl;
                _renderThreshold = 0.1;
            }
        } catch (const libconfig::SettingTypeException&) {
            std::cerr << "Warning: render.adaptive_threshold has invalid type (expected float)"
                      << std::endl;
        }
    }

    if (renderSetting.exists("ao_samples")) {
        try {
            const int a = static_cast<int>(renderSetting["ao_samples"]);
            if (a < 0) {
                std::cerr << "Warning: render.ao_samples must be >= 0, using default " << _aoSamples
                          << std::endl;
            } else {
                _aoSamples = a;
            }
        } catch (const libconfig::SettingTypeException&) {
            std::cerr << "Warning: render.ao_samples has invalid type (expected integer)"
                      << std::endl;
        }
    }

    if (renderSetting.exists("ao_max_distance")) {
        try {
            double d = static_cast<double>(renderSetting["ao_max_distance"]);
            if (d <= 0.0) {
                std::cerr << "Warning: render.ao_max_distance must be > 0, using default "
                          << _aoMaxDistance << std::endl;
            } else {
                _aoMaxDistance = d;
            }
        } catch (const libconfig::SettingTypeException&) {
            std::cerr << "Warning: render.ao_max_distance has invalid type (expected float)"
                      << std::endl;
        }
    }
}

void SceneParser::parseLights(const libconfig::Setting& setting, Scene& outScene) {
    for (int i = 0; i < setting.getLength(); ++i) {
        const libconfig::Setting& lightSetting = setting[i];
        LibconfigSetting lightConfig(lightSetting);
        if (!lightConfig.exists("type"))
            continue;

        auto lightPtr = _factories.light.create(lightConfig.getString("type"), lightConfig);
        if (lightPtr) {
            Matrix worldMatrix = _manager.getCurrentTransformation();
            if (!worldMatrix.isIdentity()) {
                lightPtr->applyTransform(worldMatrix);
            }
            outScene.addLight(std::move(lightPtr));
        }
    }
}

void SceneParser::parseMaterials(const libconfig::Setting& setting, Scene& outScene) {
    for (int i = 0; i < setting.getLength(); ++i) {
        const libconfig::Setting& mat = setting[i];
        if (!mat.exists("type") || !mat.exists("id"))
            continue;

        LibconfigSetting matConfig(mat);
        auto material = _factories.material.create(mat["type"], matConfig);
        if (material) {
            _manager.registerMaterial(mat["id"], material);
            outScene.addMaterial(_manager.getFullNamespace() + (const char*)mat["id"],
                                 std::move(material));
        }
    }
}

void SceneParser::parseSky(const libconfig::Setting& setting, Scene& outScene) {
    LibconfigSetting skyConfig(setting);
    if (!skyConfig.exists("type"))
        return;
    auto sky = _factories.sky.create(skyConfig.getString("type"), skyConfig);
    if (sky)
        outScene.setSky(std::move(sky));
    else
        outScene.setSky(std::make_unique<EmptySky>());
}

void SceneParser::parseRender(const libconfig::Setting& setting, Scene& outScene) {
    LibconfigSetting renderConfig(setting);
    _renderSamples = renderConfig.getInt("samples", _renderSamples);
    _renderThreshold = renderConfig.getFloat("threshold", _renderThreshold);
}

Matrix SceneParser::parseMatrix(const libconfig::Setting& setting) {
    LibconfigSetting config(setting);
    Vector3D pos = config.getVector("position", Vector3D(0, 0, 0));
    Vector3D rot = config.getVector("rotation", Vector3D(0, 0, 0));
    Vector3D scaleVec(1.0, 1.0, 1.0);

    if (setting.exists("scale")) {
        if (setting["scale"].isGroup()) {
            LibconfigSetting s_config(setting["scale"]);
            scaleVec.x = s_config.getFloat("x", 1.0);
            scaleVec.y = s_config.getFloat("y", 1.0);
            scaleVec.z = s_config.getFloat("z", 1.0);
        } else {
            double s = config.getFloat("scale", 1.0);
            scaleVec = Vector3D(s, s, s);
        }
    }

    Matrix m = Matrix::translate(pos.x, pos.y, pos.z);
    m = m * Matrix::rotateX(Math::degreesToRadians(rot.x));
    m = m * Matrix::rotateY(Math::degreesToRadians(rot.y));
    m = m * Matrix::rotateZ(Math::degreesToRadians(rot.z));
    m = m * Matrix::scale(scaleVec.x, scaleVec.y, scaleVec.z);
    return m;
}

} // namespace Raytracer
