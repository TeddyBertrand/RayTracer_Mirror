#pragma once

#include "factory/AFactory.hpp"
#include "factory/SceneFactories.hpp"
#include "parser/dlloader/DLLoader.hpp"
#include <filesystem>
#include <iostream>
#include <unordered_map>

namespace Raytracer {

class PluginLoader {
public:
    explicit PluginLoader(SceneFactories& factories) : _factories(factories) {}

    void loadPlugins(const std::string& path);

private:
    void loadPluginsForDirectory(const std::filesystem::directory_entry& directorypath);

    static constexpr const char* _cameraDirectory = "camera";
    static constexpr const char* _lightDirectory = "lights";
    static constexpr const char* _materialDirectory = "materials";
    static constexpr const char* _primitiveDirectory = "primitives";
    static constexpr const char* _skyDirectory = "skies";

    void handleCamera(const std::string& path);
    void handleLight(const std::string& path);
    void handleMaterial(const std::string& path);
    void handlePrimitive(const std::string& path);
    void handleSky(const std::string& path);

    using PluginHandler = void (PluginLoader::*)(const std::string& directorypath);
    using DispatchTable = std::unordered_map<std::string, PluginHandler>;

    static inline const DispatchTable _dispatchTable = {
        {_cameraDirectory, &PluginLoader::handleCamera},
        {_lightDirectory, &PluginLoader::handleLight},
        {_materialDirectory, &PluginLoader::handleMaterial},
        {_primitiveDirectory, &PluginLoader::handlePrimitive},
        {_skyDirectory, &PluginLoader::handleSky}};

    DLLoader _loader;
    SceneFactories& _factories;
};
} // namespace Raytracer
