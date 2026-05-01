#include "Logger.hpp"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>

namespace Raytracer {

Logger::~Logger() {
    if (_ofs.is_open())
        _ofs.close();
}

bool Logger::init(const std::string& dir, const std::string& prefix) {
    try {
        std::filesystem::create_directories(dir);

        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm tm{};
        localtime_r(&t, &tm);
        std::ostringstream oss;
        oss << prefix << "_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".log";

        _path = dir + "/" + oss.str();
        _ofs.open(_path, std::ios::out | std::ios::trunc);
        return _ofs.is_open();
    } catch (...) {
        return false;
    }
}

void Logger::writeLine(const std::string& line) {
    if (!_ofs.is_open())
        return;
    _ofs << line << std::endl;
}

void Logger::writeKV(const std::string& key, const std::string& value) {
    if (!_ofs.is_open())
        return;
    _ofs << key << ": " << value << std::endl;
}

} // namespace Raytracer
