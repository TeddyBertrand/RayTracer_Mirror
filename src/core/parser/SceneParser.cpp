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
    if (!setting.exists("type"))
        return nullptr;

    LibconfigSetting baseConfig(setting);

    PrimitiveSetting shapeConfig(baseConfig,
                                 _manager.getContextualMaterials(outScene.getMaterials()));

    auto primitivePtr = _factories.primitive.create(setting["type"], shapeConfig);

    if (!primitivePtr)
        return nullptr;

    auto entity = std::dynamic_pointer_cast<Entity>(primitivePtr);
    if (entity) {
        entity->setTransform(_manager.getCurrentTransformation() * entity->getTransform());
    }

    if (setting.exists("childs") && setting["childs"].isGroup()) {
        return handleChildren(setting["childs"], primitivePtr, entity, outScene);
    }

    return primitivePtr;
}

std::shared_ptr<IPrimitive> SceneParser::handleImport(const libconfig::Setting& setting,
                                                      Scene& outScene) {
    LibconfigSetting baseConfig(setting);

    std::string path = baseConfig.getString("path");
    std::string name = baseConfig.getString("name", "sub");

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
    if (parentPrimitive)
        group->add(parentPrimitive);

    if (parentEntity)
        _manager.pushTransformation(parentEntity->getLocalTransform());

    if (setting.exists("shapes")) {
        parseShapesInternal(setting["shapes"], outScene, group.get());
    }

    if (parentEntity)
        _manager.popTransformation();
    return group;
}

void SceneParser::parseCamera(const libconfig::Setting& setting, Scene& outScene) {
    LibconfigSetting cameraConfig(setting);
    if (!cameraConfig.exists("type"))
        return;

    auto camera = _factories.camera.create(cameraConfig.getString("type"), cameraConfig);

    if (camera) {
        outScene.setCamera(std::move(camera));
    }
}

void SceneParser::parseRender(const libconfig::Setting& renderSetting,
                              [[maybe_unused]] Scene& outScene) {
    LibconfigSetting renderConfig(renderSetting);

    const std::string rendererType = renderConfig.getString("type", "default");
    _renderer = _factories.renderer.create(rendererType, renderConfig);
}

void SceneParser::parsePreview(const libconfig::Setting& previewSetting, Scene& outScene) {
    (void)outScene;
    LibconfigSetting previewConfig(previewSetting);

    const std::string rendererType = previewConfig.getString("type", "fast");
    _previewRenderer = _factories.renderer.create(rendererType, previewConfig);
}

void SceneParser::parseLights(const libconfig::Setting& setting, Scene& outScene) {
    for (int i = 0; i < setting.getLength(); ++i) {
        LibconfigSetting lightConfig(setting[i]);
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

        std::string matId = matConfig.getString("id");
        std::string matType = matConfig.getString("type");

        auto material = _factories.material.create(matType, matConfig);

        if (material) {
            std::string fullKey = _manager.getFullNamespace() + matId;
            _manager.registerMaterial(matId, material);
            outScene.addMaterial(fullKey, std::move(material));
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
        outScene.setSky(std::make_shared<EmptySky>());
}

void SceneParser::parseGraphic(const libconfig::Setting& setting, Scene& outScene) {
    LibconfigSetting graphicConfig(setting);
    if (!graphicConfig.exists("type")) {
        return;
    }
    auto graphic = _factories.graphic.create(graphicConfig.getString("type"), graphicConfig);
    if (graphic) {
        outScene.setGraphic(std::move(graphic));
    }
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
