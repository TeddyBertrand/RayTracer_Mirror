#include "PluginLoader.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

void PluginLoader::loadPluginsForDirectory(const std::filesystem::directory_entry& directorypath) {
    const auto dirName = directorypath.path().filename().string();
    auto it = _dispatchTable.find(dirName);
    if (it == _dispatchTable.end()) {
        std::cerr << "PluginLoader: ignoring unknown directory '" << dirName << "'\n";
        return;
    }

    PluginHandler handler = it->second;
    (this->*handler)(directorypath.path().string());
}

void PluginLoader::loadPlugins(const std::string& path) {
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_directory(entry)) {
            loadPluginsForDirectory(entry);
        }
    }
}

template <typename CreateFn, typename Factory>
static void registerPlugins(DLLoader& loader, Factory& factory, const std::string& path) {
    using nameFunction = const char* (*)();
    for (const auto& lib : std::filesystem::directory_iterator(path)) {
        if (!std::filesystem::is_regular_file(lib))
            continue;
        const auto libPath = lib.path().string();
        try {
            loader.openLibrary(libPath);
        } catch (const std::exception& e) {
            std::cerr << "Failed to load library '" << libPath << "': " << e.what() << "\n";
            continue;
        }

        auto getName = loader.getFunction<nameFunction>("getName", libPath);
        auto create = loader.getFunction<CreateFn>("createPlugin", libPath);
        std::string name = getName ? std::string(getName()) : lib.path().stem().string();
        if (create)
            factory.registerType(name, create);
    }
}

void PluginLoader::handleCamera(const std::string& path) {
    registerPlugins<ICamera* (*)(const ISetting&), CameraFactory>(_loader, _factories.camera, path);
}

void PluginLoader::handleLight(const std::string& path) {
    registerPlugins<ILight* (*)(const ISetting&), LightFactory>(_loader, _factories.light, path);
}

void PluginLoader::handleMaterial(const std::string& path) {
    registerPlugins<IMaterial* (*)(const ISetting&), MaterialFactory>(
        _loader, _factories.material, path);
}

void PluginLoader::handlePrimitive(const std::string& path) {
    registerPlugins<IPrimitive* (*)(const ISetting&), PrimitiveFactory>(
        _loader, _factories.primitive, path);
}

void PluginLoader::handleSky(const std::string& path) {
    registerPlugins<ISky* (*)(const ISetting&), SkyFactory>(_loader, _factories.sky, path);
}

} // namespace Raytracer
