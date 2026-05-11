#include "Raytracer.hpp"
#include "core/image/Image.hpp"
#include "parser/SceneParser.hpp"
#include <filesystem>
#include <iostream>

#include <chrono>
#include <future>
#include <thread>

namespace Raytracer {

Raytracer::Raytracer(int argc, const char** argv) : _pluginLoader(_factories), _parser(_factories) {
    std::string configPath;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (configPath.empty() && arg[0] != '-') {
            configPath = arg;
        }
    }

    if (configPath.empty()) {
        std::cerr << "Error: No configuration file provided." << std::endl;
        std::cerr << "Usage: ./raytracer <config_file.cfg>" << std::endl;
        _exitCode = ERROR_STATUS;
        return;
    }

    _configPath = configPath;

    try {
        _pluginLoader.loadPlugins("plugins");
        _parser.loadScene(configPath, _scene);
        _renderer = _parser.getRenderer();
    } catch (const SceneParser::SceneParserException& e) {
        std::cerr << "Scene parser error for '" << configPath << "': " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected initialization error: " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    }
    _scene.buildBVH();
}

void Raytracer::run() {
    if (_exitCode != SUCCESS_STATUS)
        return;

    try {
        FrameBuffer frameBuffer;
        if (!_renderer) {
            std::cerr << "Error: no renderer plugin could be created from the render section"
                      << std::endl;
            _exitCode = ERROR_STATUS;
            return;
        }

        auto& camera = _scene.getCamera();

        auto renderTask =
            std::async(std::launch::async, [&]() { _renderer->render(_scene, frameBuffer); });

        _loadingBar.start();
        while (renderTask.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
            _loadingBar.update(*_renderer);
        }
        _loadingBar.finish(*_renderer);

        Image img(camera.getWidth(), camera.getHeight());
        img.drawFromBuffer(frameBuffer);

    } catch (const Scene::SceneException& e) {
        std::cerr << "Scene error while running render: " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected runtime error: " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    }
}

} // namespace Raytracer
