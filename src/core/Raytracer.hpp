#pragma once

#include <memory>
#include <string>
#include <vector>

#include "core/plugin_loader/PluginLoader.hpp"
#include "core/renderer/Renderer.hpp"
#include "core/scene/Scene.hpp"
#include "factory/SceneFactories.hpp"
#include "parser/SceneParser.hpp"

namespace Raytracer {

class Raytracer {
public:
    Raytracer(int argc, const char** argv);
    ~Raytracer() = default;

    void run();
    int getStatus() const { return _exitCode; }

private:
    static constexpr int SUCCESS_STATUS = 0;
    static constexpr int ERROR_STATUS = 84;

    int _exitCode = SUCCESS_STATUS;

    SceneFactories _factories;
    PluginLoader _pluginLoader;
    SceneParser _parser;
    Renderer _renderer;
    Scene _scene;
};
} // namespace Raytracer
