#include "Raytracer.hpp"
#include "core/image/Image.hpp"
#include "parser/SceneParser.hpp"

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

namespace Raytracer {

namespace {

volatile std::sig_atomic_t g_stopRequested = 0;

void handleSignal([[maybe_unused]] int signum) { g_stopRequested = 1; }

bool signalStopRequested() { return g_stopRequested != 0; }

void installSignalHandler() { std::signal(SIGINT, handleSignal); }

} // namespace

struct Raytracer::RenderContext {
    const ICamera& camera;
    FrameBuffer previewBuffer;
    FrameBuffer finalBuffer;
    std::vector<std::uint8_t> previewRows;
    std::vector<std::uint8_t> finalRows;
    FrameBuffer previewSnapshot;
    bool previewReady = false;
    bool hasDisplay = false;
    size_t pixelCount = 0;
};

Raytracer::Raytracer(int argc, const char** argv) : _pluginLoader(_factories), _parser(_factories) {
    installSignalHandler();

    _configPath = parseConfigPath(argc, argv);
    if (_configPath.empty()) {
        std::cerr << "Error: No configuration file provided." << std::endl;
        std::cerr << "Usage: ./raytracer <config_file.cfg>" << std::endl;
        _exitCode = ERROR_STATUS;
        return;
    }

    if (!loadSceneFromConfig())
        return;

    startFileWatcher();
}

Raytracer::~Raytracer() { stopFileWatcher(); }

std::string Raytracer::parseConfigPath(int argc, const char** argv) const {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (!arg.empty() && arg[0] != '-')
            return arg;
    }
    return {};
}

bool Raytracer::loadSceneFromConfig() {
    try {
        _pluginLoader.loadPlugins("plugins");
        _parser.loadScene(_configPath, _scene);
        _renderer = _parser.getRenderer();
        _previewRenderer = _parser.getPreviewRenderer();
        _graphic = _scene.getGraphic();
        _scene.buildBVH();
        return true;
    } catch (const SceneParser::SceneParserException& e) {
        std::cerr << "Scene parser error for '" << _configPath << "': " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected initialization error: " << e.what() << std::endl;
    }

    _exitCode = ERROR_STATUS;
    return false;
}

void Raytracer::startFileWatcher() {
    try {
        _fileWatcher = std::make_unique<FileWatcher>(_configPath);
        _fileWatcher->onFileChanged(
            [this](const std::string& path) { handleConfigFileChange(path); });
        _fileWatcherThread = std::jthread([this](std::stop_token stopToken) {
            while (!stopToken.stop_requested()) {
                if (_fileWatcher)
                    _fileWatcher->update();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        });
    } catch (const std::exception& e) {
        std::cerr << "Warning: failed to start FileWatcher: " << e.what() << std::endl;
    }
}

void Raytracer::stopFileWatcher() {
    if (_fileWatcherThread.joinable()) {
        _fileWatcherThread.request_stop();
        _fileWatcherThread.join();
    }
    _fileWatcher.reset();
}

bool Raytracer::reloadScene() {
    cleanupRenderers();
    return loadSceneFromConfig();
}

void Raytracer::handleConfigFileChange(const std::string& path) {
    std::cout << "Config file changed: " << path << std::endl;
    _reloadRequested.store(true);
}

void Raytracer::run() {
    if (_exitCode != SUCCESS_STATUS)
        return;

    try {
        while (_exitCode == SUCCESS_STATUS) {
            if (signalStopRequested()) {
                stop();
                _exitCode = ERROR_STATUS;
                break;
            }

            _reloadRequested.store(false);

            if (!renderSceneOnce())
                break;

            if (_reloadRequested.load()) {
                if (!reloadScene())
                    break;
                continue;
            }

            break;
        }
    } catch (const Scene::SceneException& e) {
        std::cerr << "Scene error while running render: " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected runtime error: " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    }
}

bool Raytracer::renderSceneOnce() {
    if (!_renderer) {
        std::cerr << "Error: no renderer plugin could be created from the render section"
                  << std::endl;
        _exitCode = ERROR_STATUS;
        return false;
    }

    const auto& camera = _scene.getCamera();
    const auto width = camera.getWidth();
    const auto height = camera.getHeight();
    const size_t pixelCount = static_cast<size_t>(width) * static_cast<size_t>(height);

    RenderContext ctx{camera,
                      FrameBuffer(pixelCount, Color(0, 0, 0)),
                      FrameBuffer(pixelCount, Color(0, 0, 0)),
                      std::vector<std::uint8_t>(height, 0),
                      std::vector<std::uint8_t>(height, 0),
                      FrameBuffer(pixelCount, Color(0, 0, 0)),
                      false,
                      false,
                      pixelCount};

    if (_graphic) {
        _graphic->setImageSize(width, height);
        ctx.hasDisplay = _graphic->init();
        if (!ctx.hasDisplay) {
            std::cerr << "Warning: Failed to initialize graphics display" << std::endl;
        }
    }

    renderPreview(ctx);
    if (_reloadRequested.load()) {
        cleanupRenderers();
        return true;
    }

    renderFinal(ctx);
    if (_reloadRequested.load()) {
        cleanupRenderers();
        return true;
    }

    if (ctx.hasDisplay && _graphic && _graphic->isOpen()) {
        showFinalFrame(ctx);
        waitForDisplayClose();
        if (_reloadRequested.load()) {
            cleanupRenderers();
            return true;
        }
    }

    Image img(width, height);
    img.drawFromBuffer(ctx.finalBuffer);
    cleanupRenderers();
    return true;
}

void Raytracer::renderPreview(RenderContext& ctx) {
    if (!_previewRenderer)
        return;

    const auto previewStart = std::chrono::steady_clock::now();
    auto previewTask = std::async(std::launch::async, [&]() {
        _previewRenderer->render(_scene, ctx.previewBuffer, &ctx.previewRows);
    });

    while (previewTask.wait_for(std::chrono::milliseconds(16)) != std::future_status::ready) {
        if (signalStopRequested()) {
            stop();
            _previewRenderer->stop();
            if (_renderer)
                _renderer->stop();
            break;
        }

        if (_reloadRequested.load()) {
            _previewRenderer->stop();
            if (_renderer)
                _renderer->stop();
            break;
        }

        if (ctx.hasDisplay && _graphic && !_graphic->isOpen()) {
            _previewRenderer->stop();
            if (_renderer)
                _renderer->stop();
            break;
        }

        updateDisplay(ctx);
        if (_previewRenderer->shouldStop())
            break;
    }

    try {
        previewTask.wait();
    } catch (...) {
    }

    ctx.previewSnapshot = ctx.previewBuffer;
    ctx.previewReady = true;

    if (ctx.hasDisplay && _graphic && _graphic->isOpen()) {
        updateDisplay(ctx);
        const auto minPreviewDuration = std::chrono::milliseconds(500);
        while (std::chrono::steady_clock::now() - previewStart < minPreviewDuration &&
               _graphic->isOpen()) {
            if (signalStopRequested()) {
                stop();
                break;
            }
            _graphic->refresh();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }
}

void Raytracer::renderFinal(RenderContext& ctx) {
    auto renderTask = std::async(
        std::launch::async, [&]() { _renderer->render(_scene, ctx.finalBuffer, &ctx.finalRows); });

    _loadingBar.start();
    while (renderTask.wait_for(std::chrono::milliseconds(16)) != std::future_status::ready) {
        if (signalStopRequested()) {
            stop();
            _renderer->stop();
            if (_previewRenderer)
                _previewRenderer->stop();
            break;
        }

        if (_reloadRequested.load()) {
            _renderer->stop();
            if (_previewRenderer) {
                _previewRenderer->stop();
            }
            break;
        }

        _loadingBar.update(*_renderer);

        if (ctx.hasDisplay) {
            if (_graphic && !_graphic->isOpen()) {
                _renderer->stop();
                if (_previewRenderer) {
                    _previewRenderer->stop();
                }
                break;
            }
            updateDisplay(ctx);
        }

        if (_renderer->shouldStop())
            break;
    }

    try {
        renderTask.wait();
    } catch (...) {
    }

    _loadingBar.finish(*_renderer);
}

void Raytracer::updateDisplay(RenderContext& ctx) {
    if (!ctx.hasDisplay || !_graphic)
        return;

    FrameBuffer composed(ctx.pixelCount, Color(0, 0, 0));

    if (!ctx.previewReady || !_previewRenderer) {
        std::scoped_lock<std::mutex> lock(getFrameBufferWriteMutex());
        composed = ctx.finalBuffer;
    } else {
        composed = ctx.previewSnapshot;
        std::scoped_lock<std::mutex> lock(getFrameBufferWriteMutex());
        for (size_t y = 0;
             y < ctx.finalRows.size() && y < static_cast<size_t>(ctx.camera.getHeight());
             ++y) {
            if (!ctx.finalRows[y])
                continue;

            const size_t rowOffset = y * static_cast<size_t>(ctx.camera.getWidth());
            for (int x = 0; x < ctx.camera.getWidth(); ++x) {
                composed[rowOffset + static_cast<size_t>(x)] =
                    ctx.finalBuffer[rowOffset + static_cast<size_t>(x)];
            }
        }
    }

    for (size_t i = 0; i < composed.size() && i < ctx.pixelCount; ++i) {
        const Color& c = composed[i];
        const int x = static_cast<int>(i % ctx.camera.getWidth());
        const int y = static_cast<int>(i / ctx.camera.getWidth());

        const unsigned int r = static_cast<unsigned int>(Color::toByte(c.r));
        const unsigned int g = static_cast<unsigned int>(Color::toByte(c.g));
        const unsigned int b = static_cast<unsigned int>(Color::toByte(c.b));
        const unsigned int a = 255;

        const unsigned int packedColor = (r << 24) | (g << 16) | (b << 8) | a;
        _graphic->updatePixel(x, y, packedColor);
    }

    _graphic->refresh();
}

void Raytracer::showFinalFrame(RenderContext& ctx) {
    if (!ctx.hasDisplay || !_graphic)
        return;

    updateDisplay(ctx);
}

void Raytracer::waitForDisplayClose() {
    if (!_graphic || !_graphic->isOpen())
        return;

    while (_graphic->isOpen()) {
        if (signalStopRequested()) {
            _graphic->close();
            break;
        }

        if (_reloadRequested.load()) {
            _graphic->close();
            break;
        }

        _graphic->refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    cleanupRenderers();
}

void Raytracer::cleanupRenderers() {
    if (_renderer) {
        _renderer->stop();
    }
    if (_previewRenderer) {
        _previewRenderer->stop();
    }
}

} // namespace Raytracer
