#pragma once
#include "core/scene/Scene.hpp"
#include "factory/SceneFactories.hpp"
#include "math/Color.hpp"
#include "math/Vector3D.hpp"
#include "parser/LibConfigSettings.hpp"
#include <iostream>
#include <libconfig.h++>
#include <unordered_map>
#include <vector>

namespace Raytracer {
class SceneParser {
public:
    /**
     * @param filePath Chemin vers le fichier .cfg à charger
     * @param outScene la référence de la scène actuelle pour pouvoir ajouter les objets
     */
    void loadScene(const std::string& filePath, Scene& outScene);

private:
    /**
     * @param camSetting La section camera du fichier
     * @param outScene la référence
     */
    void parseCamera(const libconfig::Setting& camSetting, Scene& outScene);
    /**
     * @param shapesSetting La liste (list) des formes du fichier
     * @param outScene la référence de la scène actuelle pour pouvoir ajouter les objets
     */
    void parseShapes(const libconfig::Setting& shapesSetting, Scene& outScene);

    /**
     * @param matsSetting La liste des matériaux du fichier
     * @param outScene la référence de la scène actuelle pour pouvoir ajouter les objets
     */
    void parseMaterials(const libconfig::Setting& matsSetting, Scene& outScene);

    /**
     * @param lightsSetting La liste des lights du fichiers
     * @param outScene la référence de la scène actuelle pour pouvoir ajouter les objets
     */
    void parseLights(const libconfig::Setting& lightsSetting, Scene& outScene);
    void parseEnvironment(const libconfig::Setting& environmentSetting, Scene& outScene);
    Color normalizeColor(const Color& color) const;
    void loadFactoryPlugins();
    std::unordered_map<std::string, std::shared_ptr<IMaterial>> _materials;
    SceneFactories _factories;
    std::vector<void*> _pluginHandles;
};
} // namespace Raytracer
