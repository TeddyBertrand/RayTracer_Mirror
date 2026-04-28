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

static constexpr char* _cameraDirectory = "camera";
static constexpr char* _lightDirectory = "lights";
static constexpr char* _materialDirectory = "materials";
static constexpr char* _primitiveDirectory = "primitives";
static constexpr char* _skyDirectory = "skies";

void Raytracer::loadPluginsForDirectory(const std::string& directorypath) {
    if (directorypath == _cameraDirectory) {
    }
    if (directorypath == _lightDirectory) {
    }
    if (directorypath == _materialDirectory) {
    }
    if (directorypath == _primitiveDirectory) {
    }
    if (directorypath == _skyDirectory) {
    }
}

void Raytracer::loadPlugins(const std::string& path) {
    for (const auto& libraryEntry : std::filesystem::directory_iterator(directoryPath)) {
        if (std::filesystem::is_directory(libraryEntry)) {
            loadPluginsForDirectory(libraryEntry)
        }
    }
}

void Raytracer::run() {
    if (_exitCode != SUCCESS_STATUS)
        return;

    SceneParser parser;

    try {
        parser.loadScene(_configPath, _scene);

        try {
            auto& camera = _scene.getCamera();

            FrameBuffer frameBuffer;
            _renderer.setSamples(1);

            std::cout << "[Info] Début du rendu..." << std::endl;
            _renderer.render(camera, _scene, frameBuffer);

            Image img(camera.getWidth(), camera.getHeight());
            img.drawFromBuffer(frameBuffer);
            std::cout << "[Success] Image sauvegardée dans output.ppm" << std::endl;

        } catch (const std::runtime_error& e) {
            std::cerr << "ERREUR SCÈNE : " << e.what() << std::endl;
            return;
        }

    } catch (const std::exception& e) {
        std::cerr << "ERREUR FATALE : " << e.what() << std::endl;
        _exitCode = ERROR_STATUS;
    }
}

} // namespace Raytracer
