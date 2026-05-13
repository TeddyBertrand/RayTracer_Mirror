#include "FileWatcher.hpp"

namespace Raytracer {

FileWatcher::FileWatcher(const std::string& path) : _filePath(path) {
    try {
        if (std::filesystem::exists(path))
            _lastTime = std::filesystem::last_write_time(path);
        else
            _lastTime = std::filesystem::file_time_type::min();
    } catch (...) {
        _lastTime = std::filesystem::file_time_type::min();
    }
}

void FileWatcher::update() {
    try {
        if (std::filesystem::exists(_filePath)) {
            auto currentTime = std::filesystem::last_write_time(_filePath);
            if (currentTime != _lastTime) {
                _lastTime = currentTime;
                notifyAll();
            }
        } else {
            // file missing: if previously existed, update state and notify
            if (_lastTime != std::filesystem::file_time_type::min()) {
                _lastTime = std::filesystem::file_time_type::min();
                notifyAll();
            }
        }
    } catch (...) {
        // ignore filesystem errors during polling
    }
}

void FileWatcher::notifyAll() {
    for (auto& cb : _callbacks) {
        cb(_filePath);
    }
}

} // namespace Raytracer
