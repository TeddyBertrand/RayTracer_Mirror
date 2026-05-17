#include "Scene.hpp"
#include <iomanip>
#include <iostream>

namespace Raytracer {

void Scene::dump() {
    const std::string separator(60, '=');
    const std::string blue = "\033[1;34m";
    const std::string green = "\033[1;32m";
    const std::string yellow = "\033[1;33m";
    const std::string red = "\033[1;31m";
    const std::string reset = "\033[0m";

    std::cout << "\n" << blue << separator << reset << std::endl;
    std::cout << "                RAYTRACER RUNTIME DIAGNOSTIC" << std::endl;
    std::cout << blue << separator << reset << std::endl;

    std::cout << "\n" << yellow << "[Step 1] MATERIALS MAP" << reset << std::endl;
    if (_materials.empty()) {
        std::cout << "   " << red << "/!\\ WARNING: No materials in the map." << reset << std::endl;
    } else {
        for (auto const& [name, mat] : _materials) {
            std::cout << "   - [" << std::left << std::setw(15) << name << "] -> "
                      << (mat ? green + "Ready" : red + "NULL") << reset << std::endl;
        }
    }

    std::cout << "\n" << yellow << "[Step 2] CAMERA & RAYCASTING" << reset << std::endl;
    if (_camera) {
        Ray testRay = _camera->getRay(0.5, 0.5);
        auto orig = testRay.origin();
        auto dir = testRay.direction();

        std::cout << "   - Resolution : " << _camera->getWidth() << "x" << _camera->getHeight()
                  << std::endl;
        std::cout << "   - Origin     : (" << orig.x << ", " << orig.y << ", " << orig.z << ")"
                  << std::endl;
        std::cout << "   - Direction  : (" << dir.x << ", " << dir.y << ", " << dir.z << ")"
                  << std::endl;

        if (dir.x == 0 && dir.y == 0 && dir.z == 0)
            std::cout << "   " << red << "[!] ALERT: Camera direction is NULL (0,0,0)!" << reset
                      << std::endl;
    } else {
        std::cout << "   " << red << "[!] ERROR: No camera detected." << reset << std::endl;
    }

    std::cout << "\n" << yellow << "[Step 3] LIGHTING EVALUATION" << reset << std::endl;
    if (_lights.empty()) {
        std::cout << "   " << red << "[!] WARNING: No light sources found." << reset << std::endl;
    } else {
        for (size_t i = 0; i < _lights.size(); ++i) {
            auto sample = _lights[i]->computeLight(HitRecord{});
            std::cout << "   - Light #" << i << " : Dist=" << std::fixed << std::setprecision(2)
                      << sample.distance << ", Color=(" << sample.color.r << "," << sample.color.g
                      << "," << sample.color.b << ")"
                      << " [" << (sample.isActive ? green + "ON" : red + "OFF") << reset << "]"
                      << std::endl;
        }
    }

    std::cout << "\n" << yellow << "[Step 4] WORLD HIERARCHY" << reset << std::endl;
    _world.dump(3);

    std::cout << "\n" << blue << separator << reset << "\n" << std::endl;
}

} // namespace Raytracer
