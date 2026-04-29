#pragma once
#include "CameraFactory.hpp"
#include "LightFactory.hpp"
#include "MaterialFactory.hpp"
#include "PrimitiveFactory.hpp"
#include "SkyFactory.hpp"

namespace Raytracer {
/**
 * @brief Regroupement des factories pour la construction de la scène.
 * * Ce header permet d'inclure d'un coup l'ensemble des usines (Camera,
 * Lights, Materials, Primitives) nécessaires au SceneParser.
 */
struct SceneFactories {
    CameraFactory camera;
    LightFactory light;
    MaterialFactory material;
    PrimitiveFactory primitive;
    SkyFactory sky;
};
} // namespace Raytracer
