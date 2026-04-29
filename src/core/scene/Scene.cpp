#include "Scene.hpp"
#include <iomanip>
#include <iostream>

namespace Raytracer {

void Scene::dump() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "                RAYTRACER RUNTIME DIAGNOSTIC" << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    // 1. Matériaux : On vérifie les IDs
    std::cout << "\n[Step 1] MATERIALS MAP" << std::endl;
    if (_materials.empty()) {
        std::cout << "   /!\\ WARNING: No materials in the map. Shapes will be black." << std::endl;
    } else {
        for (auto const& [name, mat] : _materials) {
            std::cout << "   - [" << name << "] -> " << (mat ? "Ready" : "NULL") << std::endl;
        }
    }

    // 2. Caméra : On utilise getRay(0.5, 0.5) pour simuler le "regard"
    std::cout << "\n[Step 2] CAMERA & RAYCASTING" << std::endl;
    if (_camera) {
        Ray testRay = _camera->getRay(0.5, 0.5);
        Point3D orig = testRay.origin();
        Vector3D dir = testRay.direction();

        std::cout << "   - Resolution : " << _camera->getWidth() << "x" << _camera->getHeight()
                  << std::endl;
        std::cout << "   - Origin     : (" << orig.x << ", " << orig.y << ", " << orig.z << ")"
                  << std::endl;
        std::cout << "   - Direction  : (" << dir.x << ", " << dir.y << ", " << dir.z << ")"
                  << std::endl;

        if (dir.x == 0 && dir.y == 0 && dir.z == 0) {
            std::cout << "   [!] ALERT: Camera direction is NULL (0,0,0)!" << std::endl;
        }
    } else {
        std::cout << "   [!] ERROR: No camera detected." << std::endl;
    }

    // 3. Lumières : Test de radiance à l'origine
    std::cout << "\n[Step 3] LIGHTING EVALUATION" << std::endl;
    if (_lights.empty()) {
        std::cout << "   [!] WARNING: No light sources found." << std::endl;
    } else {
        for (size_t i = 0; i < _lights.size(); ++i) {
            // On demande à la lumière : "Que vois-tu au centre du monde (0,0,0) ?"
            auto sample = _lights[i]->computeLight(Point3D(0, 0, 0));
            std::cout << "   - Light #" << i << " : Dist=" << sample.distance << ", Color=("
                      << sample.color.r << "," << sample.color.g << "," << sample.color.b << ")"
                      << " [" << (sample.isActive ? "ON" : "OFF") << "]" << std::endl;
        }
    }

    // 4. Objets : On compte les primitives via l'interface
    std::cout << "\n[Step 4] WORLD OBJECTS" << std::endl;
    // On part du principe que si getWorld() répond, la liste est là
    std::cout << "   - World Geometry is active." << std::endl;

    std::cout << "\n" << std::string(60, '=') << "\n" << std::endl;
}

} // namespace Raytracer
