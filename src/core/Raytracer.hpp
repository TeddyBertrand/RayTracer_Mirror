#pragma once

#include <memory>
#include <string>
#include <vector>

#include "components/IGraphic.hpp"
#include "components/IRenderer.hpp"
#include "core/display/loading_bar/LoadingBar.hpp"
#include "core/file_watcher/FileWatcher.hpp"
#include "core/plugin_loader/PluginLoader.hpp"
#include "core/scene/Scene.hpp"
#include "factory/SceneFactories.hpp"
#include "parser/SceneParser.hpp"
#include <atomic>
#include <thread>

namespace Raytracer {

class Raytracer {
public:
    Raytracer(int argc, const char** argv);
    ~Raytracer();

    void run();
    int getStatus() const { return _exitCode; }

    void stop() {
        if (_renderer) {
            _renderer->stop();
        }
        if (_previewRenderer) {
            _previewRenderer->stop();
        }
        if (_graphic) {
            _graphic->close();
        }
    }

private:
    static constexpr int SUCCESS_STATUS = 0;
    static constexpr int ERROR_STATUS = 84;

    int _exitCode = SUCCESS_STATUS;

    SceneFactories _factories;
    PluginLoader _pluginLoader;
    SceneParser _parser;
    std::shared_ptr<IRenderer> _renderer;
    std::shared_ptr<IRenderer> _previewRenderer;
    std::shared_ptr<IGraphic> _graphic;
    Scene _scene;
    LoadingBar _loadingBar;
    std::string _configPath;
    std::unique_ptr<FileWatcher> _fileWatcher;
    std::jthread _fileWatcherThread;
    std::atomic<bool> _reloadRequested{false};

    void handleConfigFileChange(const std::string& path);
    std::string parseConfigPath(int argc, const char** argv) const;
    void startFileWatcher();
    void stopFileWatcher();
    bool loadSceneFromConfig();
    bool reloadScene();
    bool renderSceneOnce();

    struct RenderContext;
    void renderPreview(RenderContext& ctx);
    void renderFinal(RenderContext& ctx);
    void updateDisplay(RenderContext& ctx);
    void showFinalFrame(RenderContext& ctx);
    void waitForDisplayClose();
    void cleanupRenderers();
};
} // namespace Raytracer
