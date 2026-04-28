#include "Raytracer.hpp"
#include "core/image/Image.hpp"
#include "parser/SceneParser.hpp"
#include <filesystem>
#include <iostream>

namespace Raytracer {

Raytracer::Raytracer(int argc, const char** argv) {
    if (argc < 2) {
        std::cerr << "Erreur : Aucun fichier de configuration fourni." << std::endl;
        std::cerr << "Usage: ./raytracer <config_file.cfg>" << std::endl;
        _exitCode = ERROR_STATUS;
        return;
    }

    _configPath = argv[1];

    loadPlugins("./plugins");
}

void Raytracer::loadPlugins(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        std::cerr << "[Warning] Dossier plugins introuvable : " << path << std::endl;
        return;
    }

    std::cout << "[Info] Chargement des plugins depuis : " << path << std::endl;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
        if (!entry.is_regular_file())
            continue;

        std::string ext = entry.path().extension().string();
        if (ext != ".so" && ext != ".dll")
            continue;

        std::string libPath = entry.path().string();
        bool pluginLoaded = false;

        if (auto primFunc = _loader.getPrimitiveFunction("registerPlugin", libPath)) {
            primFunc(_factories.primitive);
            pluginLoaded = true;
        } else if (auto lightFunc = _loader.getLightFunction("registerPlugin", libPath)) {
            lightFunc(_factories.light);
            pluginLoaded = true;
        } else if (auto matFunc = _loader.getMaterialFunction("registerPlugin", libPath)) {
            matFunc(_factories.material);
            pluginLoaded = true;
        } else if (auto camFunc = _loader.getCameraFunction("registerPlugin", libPath)) {
            camFunc(_factories.camera);
            pluginLoaded = true;
        }

        if (pluginLoaded) {
            std::cout << "  [OK] Plugin chargé : " << libPath << std::endl;
        } else {
            std::cerr << "  [Error] Impossible de lier le plugin : " << libPath << std::endl;
        }
    }
}

void Raytracer::run() {
    if (_exitCode != SUCCESS_STATUS)
        return;

    SceneParser parser;

    try {
        parser.loadScene(_configPath, _scene);
        auto& camera = _scene.getCamera();
        FrameBuffer frameBuffer;

        _renderer.setSamples(1);
        _renderer.render(camera, _scene, frameBuffer);

        Image img(camera.getWidth(), camera.getHeight());
        img.drawFromBuffer(frameBuffer);

    } catch (const std::exception& e) {
        std::cerr << "ERREUR FATALE : " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    }
}
} // namespace Raytracer
