#include "Raytracer.hpp"
#include "core/image/Image.hpp"
#include "parser/SceneParser.hpp"
#include <atomic>
#include <chrono>
#include <csignal>
#include <filesystem>
#include <future>
#include <iostream>
#include <thread>

namespace Raytracer {

Raytracer* Raytracer::_instance = nullptr;

void handleSignal([[maybe_unused]] int signum) {
    if (Raytracer::getInstance()) {
        Raytracer::getInstance()->stop();
    }
}

Raytracer::Raytracer(int argc, const char** argv) : _pluginLoader(_factories), _parser(_factories) {
    _instance = this;

    std::signal(SIGINT, handleSignal);

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
        _previewRenderer = _parser.getPreviewRenderer();
        _graphic = _scene.getGraphic();
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
        if (!_renderer) {
            std::cerr << "Error: no renderer plugin could be created from the render section"
                      << std::endl;
            _exitCode = ERROR_STATUS;
            return;
        }

        auto& camera = _scene.getCamera();
        const size_t pixelCount = static_cast<size_t>(camera.getWidth()) * camera.getHeight();
        FrameBuffer previewBuffer(pixelCount, Color(0, 0, 0));
        FrameBuffer finalBuffer(pixelCount, Color(0, 0, 0));
        std::vector<std::uint8_t> previewRows(camera.getHeight(), 0);
        std::vector<std::uint8_t> finalRows(camera.getHeight(), 0);

        bool hasDisplay = false;
        if (_graphic) {
            _graphic->setImageSize(camera.getWidth(), camera.getHeight());
            hasDisplay = _graphic->init();
            if (!hasDisplay) {
                std::cerr << "Warning: Failed to initialize graphics display" << std::endl;
            }
        }

        FrameBuffer previewSnapshot;
        bool previewReady = false;

        auto presentFrame = [&](const FrameBuffer& previewSource,
                                const FrameBuffer& finalSource,
                                const std::vector<std::uint8_t>& finalMask,
                                bool usePreviewBase) {
            if (!hasDisplay) {
                return;
            }

            FrameBuffer composed(pixelCount, Color(0, 0, 0));

            if (!usePreviewBase || !_previewRenderer || !previewReady) {
                std::scoped_lock<std::mutex> lock(getFrameBufferWriteMutex());
                composed = finalSource;
            } else {
                composed = previewSnapshot;
                std::scoped_lock<std::mutex> lock(getFrameBufferWriteMutex());
                for (size_t y = 0;
                     y < finalMask.size() && y < static_cast<size_t>(camera.getHeight());
                     ++y) {
                    if (!finalMask[y]) {
                        continue;
                    }
                    const size_t rowOffset = y * static_cast<size_t>(camera.getWidth());
                    for (int x = 0; x < camera.getWidth(); ++x) {
                        composed[rowOffset + static_cast<size_t>(x)] =
                            finalSource[rowOffset + static_cast<size_t>(x)];
                    }
                }
            }

            for (size_t i = 0; i < composed.size() && i < pixelCount; ++i) {
                const Color& c = composed[i];
                const int x = static_cast<int>(i % camera.getWidth());
                const int y = static_cast<int>(i / camera.getWidth());

                const unsigned int r = static_cast<unsigned int>(Color::toByte(c.r));
                const unsigned int g = static_cast<unsigned int>(Color::toByte(c.g));
                const unsigned int b = static_cast<unsigned int>(Color::toByte(c.b));
                const unsigned int a = 255;

                unsigned int packedColor = (r << 24) | (g << 16) | (b << 8) | a;
                _graphic->updatePixel(x, y, packedColor);
            }
            _graphic->refresh();
        };

        std::future<void> previewTask;
        if (_previewRenderer) {
            const auto previewStart = std::chrono::steady_clock::now();
            previewTask = std::async(std::launch::async, [&]() {
                _previewRenderer->render(_scene, previewBuffer, &previewRows);
            });

            while (previewTask.wait_for(std::chrono::milliseconds(16)) !=
                   std::future_status::ready) {
                if (hasDisplay) {
                    if (!_graphic->isOpen()) {
                        _previewRenderer->stop();
                        _renderer->stop();
                        break;
                    }
                    presentFrame(previewBuffer, finalBuffer, finalRows, false);
                }
                if (_previewRenderer->shouldStop()) {
                    break;
                }
            }

            try {
                previewTask.wait();
            } catch (...) {
            }

            previewSnapshot = previewBuffer;
            previewReady = true;

            if (hasDisplay && _graphic->isOpen()) {
                presentFrame(previewBuffer, finalBuffer, finalRows, false);

                const auto minPreviewDuration = std::chrono::milliseconds(500);
                while (std::chrono::steady_clock::now() - previewStart < minPreviewDuration &&
                       _graphic->isOpen()) {
                    _graphic->refresh();
                    std::this_thread::sleep_for(std::chrono::milliseconds(16));
                }
            }
        }

        auto renderTask = std::async(std::launch::async,
                                     [&]() { _renderer->render(_scene, finalBuffer, &finalRows); });

        _loadingBar.start();
        while (renderTask.wait_for(std::chrono::milliseconds(16)) != std::future_status::ready) {
            _loadingBar.update(*_renderer);

            if (hasDisplay) {
                if (!_graphic->isOpen()) {
                    _renderer->stop();
                    if (_previewRenderer) {
                        _previewRenderer->stop();
                    }
                    break;
                }

                presentFrame(previewBuffer, finalBuffer, finalRows, previewReady);
            }

            if (_renderer->shouldStop()) {
                break;
            }
        }

        try {
            renderTask.wait();
        } catch (...) {
        }
        if (previewTask.valid()) {
            try {
                previewTask.wait();
            } catch (...) {
            }
        }
        _loadingBar.finish(*_renderer);

        if (hasDisplay && _graphic->isOpen()) {
            presentFrame(previewBuffer, finalBuffer, finalRows, previewReady);
        }

        if (hasDisplay && _graphic->isOpen()) {
            while (_graphic->isOpen()) {
                _graphic->refresh();
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }
            _renderer->stop();
            if (_previewRenderer) {
                _previewRenderer->stop();
            }
        }

        Image img(camera.getWidth(), camera.getHeight());
        img.drawFromBuffer(finalBuffer);

    } catch (const Scene::SceneException& e) {
        std::cerr << "Scene error while running render: " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected runtime error: " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    }
}

} // namespace Raytracer
