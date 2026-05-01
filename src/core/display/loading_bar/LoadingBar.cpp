#include "LoadingBar.hpp"

#include <iomanip>
#include <iostream>

namespace Raytracer {

void LoadingBar::printRenderInfo(int width, int height, int samples) {
    std::cout << "\033[1;36m" << std::string(60, '=') << "\033[0m" << std::endl;
    std::cout << "\033[1;36m" << "Starting Render" << "\033[0m" << std::endl;
    std::cout << "\033[1;36m" << std::string(60, '=') << "\033[0m" << std::endl;
    std::cout << "  \033[1;37mResolution: \033[0m" << width << " x " << height << " pixels"
              << std::endl;
    std::cout << "  \033[1;37mSamples: \033[0m" << samples << " per pixel" << std::endl;
    std::cout << "\033[1;36m" << std::string(60, '-') << "\033[0m" << std::endl;
}

void LoadingBar::update(const Renderer& renderer) {
    int current = renderer.getCompletedRows();
    int total = renderer.getTotalRows();

    if (current == 0)
        return;

    auto now = std::chrono::steady_clock::now();
    auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - _start_time).count();

    double progress = static_cast<double>(current) / total;
    double rows_per_ms = static_cast<double>(current) / (elapsed_ms > 0 ? elapsed_ms : 1);
    double ms_left = (total - current) / rows_per_ms;

    progress = (progress > 1.0) ? 1.0 : progress;

    std::string bar = createBar(progress);
    int percent = static_cast<int>(progress * 100.0);

    std::cout << "\r\033[K"
              << "\033[1;36m➤ Rendering \033[0m" << bar << " "
              << "\033[1;37m" << std::setw(3) << percent << "%\033[0m "
              << "\033[1;37m(" << current << "/" << total << " rows)\033[0m "
              << "\033[1;34m⏱ " << formatTime(ms_left) << " left\033[0m" << std::flush;
}

void LoadingBar::finish(const Renderer& renderer) {
    int total = renderer.getTotalRows();
    auto now = std::chrono::steady_clock::now();
    auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - _start_time).count();

    std::string bar = createBar(1.0);

    std::cout << "\r\033[K"
              << "\033[1;36m➤ Rendering \033[0m" << bar << " "
              << "\033[1;32m100% (" << total << "/" << total << " rows)\033[0m "
              << "\033[1;32m✓ Completed in " << formatTime(elapsed_ms) << "\033[0m" << std::endl;
}

std::string LoadingBar::createBar(double progress) {
    int width = 40;
    int filled = static_cast<int>(width * progress);
    std::string s = "[";

    for (int i = 0; i < width; ++i) {
        if (i < filled) {
            s += "\033[1;32m█\033[0m";
        } else {
            s += "\033[1;30m░\033[0m";
        }
    }
    s += "]";
    return s;
}

} // namespace Raytracer
