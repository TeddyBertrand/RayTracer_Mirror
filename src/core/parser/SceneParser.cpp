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
            DBG("Section 'materials' détectée, parsing...");
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

    } catch (const libconfig::SettingNotFoundException& e) {
        _manager.untrackFile(filePath);
        throw SceneParserException(std::string("Error loading ") + filePath + ": " + e.what());
    } catch (const std::exception& e) {
        _manager.untrackFile(filePath);
        throw SceneParserException(std::string("Error loading ") + filePath + ": " + e.what());
    }
}

void SceneParser::parseShapes(const libconfig::Setting& setting, Scene& outScene) {
    parseShapesInternal(setting, outScene, nullptr);
}

void SceneParser::parseShapesInternal(const libconfig::Setting& setting,
                                      Scene& outScene,
                                      PrimitiveGroup* targetGroup) {

    for (int i = 0; i < setting.getLength(); ++i) {
        const libconfig::Setting& shapeSetting = setting[i];

        if (shapeSetting.exists("path")) {
            handleImport(shapeSetting, outScene);

        } else if (shapeSetting.exists("type")) {
            std::string typeName = (const char*)shapeSetting["type"];
            auto primitive = handleStandardPrimitive(shapeSetting, outScene);
            if (primitive) {
                if (targetGroup) {
                    targetGroup->add(std::move(primitive));
                } else {
                    outScene.addPrimitive(std::move(primitive));
                }
            }
        }
    }
}

std::shared_ptr<IPrimitive> SceneParser::handleStandardPrimitive(const libconfig::Setting& setting,
                                                                 Scene& outScene) {

    if (!setting.exists("type")) {
        return nullptr;
    }

    LibconfigSetting baseConfig(setting);

    if (setting.exists("material")) {
        std::string matId = (const char*)setting["material"];
        const auto& ctxMats = _manager.getContextualMaterials(outScene.getMaterials());

        PrimitiveSetting shapeConfig(baseConfig,
                                     _manager.getContextualMaterials(outScene.getMaterials()));

        std::shared_ptr<IPrimitive> primitivePtr;

        if (!primitivePtr) {
            return nullptr;
        }

        auto entity = std::dynamic_pointer_cast<Entity>(primitivePtr);
        if (entity) {
            entity->setTransform(_manager.getCurrentTransformation() * entity->getTransform());
        }

        if (setting.exists("childs")) {
            bool isGrp = setting["childs"].isGroup();
            if (isGrp) {
                return handleChildren(setting["childs"], primitivePtr, entity, outScene);
            }
        }

        return primitivePtr;
    }
}

std::shared_ptr<IPrimitive> SceneParser::handleImport(const libconfig::Setting& setting,
                                                      Scene& outScene) {
    std::string path = setting["path"];
    std::string name = setting.exists("name") ? (const char*)setting["name"] : "sub";

    _manager.pushNamespace(name);
    _manager.pushTransformation(parseMatrix(setting));
    loadScene(path, outScene);
    _manager.popTransformation();
    _manager.popNamespace();
    return nullptr;
}

std::shared_ptr<IPrimitive> SceneParser::handleChildren(const libconfig::Setting& setting,
                                                        std::shared_ptr<IPrimitive> parentPrimitive,
                                                        std::shared_ptr<Entity> parentEntity,
                                                        Scene& outScene) {
    auto group = std::make_shared<PrimitiveGroup>();

    if (parentPrimitive) {
        group->add(parentPrimitive);
    }

    if (parentEntity) {
        _manager.pushTransformation(parentEntity->getLocalTransform());
    }

    if (setting.exists("shapes")) {
        parseShapesInternal(setting["shapes"], outScene, group.get());
    }
    if (parentEntity) {
        _manager.popTransformation();
    }
    return group;
}

void SceneParser::parseCamera(const libconfig::Setting& setting, Scene& outScene) {
    LibconfigSetting cameraConfig(setting);
    if (!cameraConfig.exists("type")) {
        return;
    }
    std::string camType = cameraConfig.getString("type");

    std::shared_ptr<ICamera> camera;
    if (camera)
        outScene.setCamera(std::move(camera));
}

void SceneParser::parseRender(const libconfig::Setting& renderSetting, Scene& outScene) {
    (void)outScene;
    LibconfigSetting renderConfig(renderSetting);

    const std::string rendererType = renderConfig.getString("type", "default");

    try {
        if (renderSetting.exists("samples")) {
            const int s = static_cast<int>(renderSetting["samples"]);
            if (s < 1) {
                throw RenderSettingsException("render.samples must be >= 1 (received " +
                                              std::to_string(s) + ")");
            } else if (s > 100000) {
                std::cerr << "Warning: render.samples too large, clamping to 100000\n";
                _renderSamples = 100000;
            } else {
                _renderSamples = s;
            }
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
                std::cerr << "Warning: render.adaptive_threshold hors [0,1], défaut 0.1\n";
                _renderThreshold = 0.1;
            }
        } catch (const libconfig::SettingTypeException&) {
            std::cerr << "Warning: render.adaptive_threshold type invalide\n";
        }
    }

    if (renderSetting.exists("ao_samples")) {
        try {
            const int a = static_cast<int>(renderSetting["ao_samples"]);
            if (a < 0) {
                std::cerr << "Warning: render.ao_samples doit être >= 0\n";
            } else {
                _aoSamples = a;
            }
        } catch (const libconfig::SettingTypeException&) {
            std::cerr << "Warning: render.ao_samples type invalide\n";
        }
    }

    if (renderSetting.exists("ao_max_distance")) {
        try {
            double d = static_cast<double>(renderSetting["ao_max_distance"]);
            if (d <= 0.0) {
                std::cerr << "Warning: render.ao_max_distance doit être > 0\n";
            } else {
                _aoMaxDistance = d;
            }
        } catch (const libconfig::SettingTypeException&) {
            std::cerr << "Warning: render.ao_max_distance type invalide\n";
        }
    }
}

void SceneParser::parseLights(const libconfig::Setting& setting, Scene& outScene) {

    for (int i = 0; i < setting.getLength(); ++i) {
        const libconfig::Setting& lightSetting = setting[i];
        LibconfigSetting lightConfig(lightSetting);

        std::shared_ptr<ILight> lightPtr;

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
        if (!mat.exists("type") || !mat.exists("id")) {
            continue;
        }
        std::string matType = (const char*)mat["type"];
        std::string matId = (const char*)mat["id"];
        std::string fullKey = _manager.getFullNamespace() + matId;

        LibconfigSetting matConfig(mat);
        std::shared_ptr<IMaterial> material;

        if (material) {
            _manager.registerMaterial(matId, material);
            outScene.addMaterial(fullKey, std::move(material));
        }
    }
}

void SceneParser::parseSky(const libconfig::Setting& setting, Scene& outScene) {
    LibconfigSetting skyConfig(setting);
    if (!skyConfig.exists("type")) {
        return;
    }

    std::shared_ptr<ISky> sky;

    if (sky)
        outScene.setSky(std::move(sky));
    else
        outScene.setSky(std::make_unique<EmptySky>());
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
