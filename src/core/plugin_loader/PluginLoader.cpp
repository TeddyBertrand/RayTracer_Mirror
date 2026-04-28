#include "PluginLoader.hpp"

namespace Raytracer {

void PluginLoader::findType(const std::filesystem::directory_entry& directorypath) {
    for (const auto& libraryEntry : std::filesystem::directory_iterator(directorypath)) {
        const auto& path = libraryEntry.path();
        _loader.openLibrary(path.string().c_str());
        _factories.
    }
}

void PluginLoader::loadPluginsForDirectory(const std::filesystem::directory_entry& directorypath) {
    for (const auto& libraryEntry : std::filesystem::directory_iterator(directorypath)) {
        const auto& path = libraryEntry.path();
        _loader.openLibrary(path.string().c_str());
        _factories.
    }
}

void PluginLoader::loadPlugins(const std::string& path) {
    for (const auto& libraryEntry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_directory(libraryEntry)) {
            loadPluginsForDirectory(libraryEntry);
        }
    }
}

} // namespace Raytracer
