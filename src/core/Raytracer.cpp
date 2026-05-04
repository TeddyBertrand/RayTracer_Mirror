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
    std::string logDir = "logs";
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.rfind("--log=", 0) == 0) {
            _logEnabled = true;
            logDir = arg.substr(6);
        } else if (arg == "--log") {
            _logEnabled = true;
        } else if (configPath.empty()) {
            configPath = arg;
        }
    }

    if (configPath.empty()) {
        std::cerr << "Error: No configuration file provided." << std::endl;
        std::cerr << "Usage: ./raytracer <config_file.cfg> [--log|--log=<dir>]" << std::endl;
        _exitCode = ERROR_STATUS;
        return;
    }

    _configPath = configPath;

    if (_logEnabled) {
        _logger = std::make_unique<Logger>();
        if (!_logger->init(logDir)) {
            std::cerr << "Warning: failed to initialize logger at '" << logDir << "'" << std::endl;
            _logEnabled = false;
            _logger.reset();
        }
    }

    try {
        _pluginLoader.loadPlugins("plugins");
        _parser.loadScene(configPath, _scene);
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
        int samples = _parser.getRenderSamples();
        double threshold = _parser.getRenderThreshold();
        _renderer.setSamples(samples);
        _renderer.setAdaptiveThreshold(threshold);
        auto& camera = _scene.getCamera();

        auto renderTask = std::async(std::launch::async,
                                     [&]() { _renderer.render(camera, _scene, frameBuffer); });

        auto start_time = std::chrono::steady_clock::now();
        if (_logEnabled && _logger && _logger->isOpen()) {
            _logger->writeLine("--- Render Start ---");
            _logger->writeKV("Config", _configPath);
            _logger->writeKV("Resolution",
                             std::to_string(camera.getWidth()) + "x" +
                                 std::to_string(camera.getHeight()));
            _logger->writeKV("Samples", std::to_string(samples));
            _logger->writeKV("StartTimestamp",
                             std::to_string(std::chrono::duration_cast<std::chrono::seconds>(
                                                std::chrono::system_clock::now().time_since_epoch())
                                                .count()));
        }

        _loadingBar.start();
        while (renderTask.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
            _loadingBar.update(_renderer);
        }
        _loadingBar.finish(_renderer);

        Image img(camera.getWidth(), camera.getHeight());
        img.drawFromBuffer(frameBuffer);

        auto end_time = std::chrono::steady_clock::now();
        auto elapsed_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        if (_logEnabled && _logger && _logger->isOpen()) {
            _logger->writeKV("TotalRows", std::to_string(camera.getHeight()));
            _logger->writeKV("ElapsedMs", std::to_string(elapsed_ms));
            _logger->writeLine("--- Render End ---");
        }
    } catch (const Scene::SceneException& e) {
        std::cerr << "Scene error while running render: " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected runtime error: " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    }
}

} // namespace Raytracer
