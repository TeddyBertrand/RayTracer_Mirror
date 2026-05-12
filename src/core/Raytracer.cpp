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

        const auto& camera = _scene.getCamera();
        size_t pixelCount = static_cast<size_t>(camera.getWidth()) * camera.getHeight();

        RenderContext ctx{
            .camera = camera,
            .pixelCount = pixelCount,
        };

        ctx.previewBuffer = FrameBuffer(pixelCount, Color(0, 0, 0));
        ctx.finalBuffer = FrameBuffer(pixelCount, Color(0, 0, 0));
        ctx.previewRows = std::vector<std::uint8_t>(camera.getHeight(), 0);
        ctx.finalRows = std::vector<std::uint8_t>(camera.getHeight(), 0);

        if (_graphic) {
            _graphic->setImageSize(camera.getWidth(), camera.getHeight());
            ctx.hasDisplay = _graphic->init();
            if (!ctx.hasDisplay) {
                std::cerr << "Warning: Failed to initialize graphics display" << std::endl;
            }
        }

        renderPreview(ctx);

        renderFinal(ctx);

        if (ctx.hasDisplay && _graphic && _graphic->isOpen()) {
            showFinalFrame(ctx);
            waitForDisplayClose();
        }

        Image img(camera.getWidth(), camera.getHeight());
        img.drawFromBuffer(ctx.finalBuffer);

    } catch (const Scene::SceneException& e) {
        std::cerr << "Scene error while running render: " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected runtime error: " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    }
}

void Raytracer::renderPreview(RenderContext& ctx) {
    if (!_previewRenderer)
        return;

    const auto previewStart = std::chrono::steady_clock::now();
    auto previewTask = std::async(std::launch::async, [&]() {
        _previewRenderer->render(_scene, ctx.previewBuffer, &ctx.previewRows);
    });

    while (previewTask.wait_for(std::chrono::milliseconds(16)) != std::future_status::ready) {
        if (ctx.hasDisplay && _graphic && !_graphic->isOpen()) {
            _previewRenderer->stop();
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

        unsigned int packedColor = (r << 24) | (g << 16) | (b << 8) | a;
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
