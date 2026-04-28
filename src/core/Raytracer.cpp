#include "Raytracer.hpp"
#include "core/image/Image.hpp"
#include "parser/SceneParser.hpp"
#include <filesystem>
#include <iostream>

namespace Raytracer {

Raytracer::Raytracer(int argc, const char** argv)
    : _pluginLoader(_factories), _parser(_factories) {
    if (argc < 2) {
        std::cerr << "Erreur : Aucun fichier de configuration fourni." << std::endl;
        std::cerr << "Usage: ./raytracer <config_file.cfg>" << std::endl;
        _exitCode = ERROR_STATUS;
        return;
    }

    std::string configPath = argv[1];
    try {
        _pluginLoader.loadPlugins("plugins");
        _parser.loadScene(configPath, _scene);
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement de la scène : " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    }
}

void Raytracer::run() {
    if (_exitCode != SUCCESS_STATUS)
        return;

    FrameBuffer frameBuffer;
    _renderer.setSamples(1);
    auto& camera = _scene.getCamera();
    std::cout << "[Info] Début du rendu..." << std::endl;
    _renderer.render(camera, _scene, frameBuffer);

    Image img(camera.getWidth(), camera.getHeight());
    img.drawFromBuffer(frameBuffer);
}

} // namespace Raytracer
