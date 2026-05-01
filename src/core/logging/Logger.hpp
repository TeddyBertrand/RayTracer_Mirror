#pragma once

#include <fstream>
#include <string>

namespace Raytracer {

class Logger {
public:
    Logger() = default;
    ~Logger();

    bool init(const std::string& dir, const std::string& prefix = "render");

    void writeLine(const std::string& line);

    bool isOpen() const { return _ofs.is_open(); }

    void writeKV(const std::string& key, const std::string& value);

private:
    std::ofstream _ofs;
    std::string _path;
};

} // namespace Raytracer
