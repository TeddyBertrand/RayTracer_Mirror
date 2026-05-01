#pragma once

#include "core/renderer/Renderer.hpp"
#include <chrono>

namespace Raytracer {

class LoadingBar {
public:
    void start() { _start_time = std::chrono::steady_clock::now(); }

    void printRenderInfo(int width, int height, int samples);
    void update(const Renderer& renderer);
    void finish(const Renderer& renderer);

private:
    std::chrono::steady_clock::time_point _start_time;

    std::string createBar(double progress);

    std::string formatTime(double ms) {
        int minutes = static_cast<int>(ms / 60000.0);
        int seconds = static_cast<int>((ms - minutes * 60000) / 1000.0);
        int millis = static_cast<int>(ms) % 1000;

        std::string result;
        if (minutes > 0)
            result += std::to_string(minutes) + "m ";
        result += std::to_string(seconds) + "s ";
        result += std::to_string(millis) + "ms";
        return result;
    }
};

} // namespace Raytracer
