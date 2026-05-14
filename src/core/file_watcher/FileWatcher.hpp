#pragma once

#include <chrono>
#include <filesystem>
#include <functional>
#include <string>
#include <thread>
#include <vector>

namespace Raytracer {

class FileWatcher {
public:
    using FileCallback = std::function<void(const std::string&)>;

    FileWatcher(const std::string& path);

    void onFileChanged(FileCallback callback) { _callbacks.push_back(callback); }

    void update();

private:
    void notifyAll();

    std::string _filePath;
    std::filesystem::file_time_type _lastTime;
    std::vector<FileCallback> _callbacks;
};

} // namespace Raytracer
