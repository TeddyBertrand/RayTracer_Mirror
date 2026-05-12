#pragma once
#include "CameraFactory.hpp"
#include "GraphicFactory.hpp"
#include "LightFactory.hpp"
#include "MaterialFactory.hpp"
#include "PrimitiveFactory.hpp"
#include "RendererFactory.hpp"
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
    RendererFactory renderer;
    GraphicFactory graphic;
};
} // namespace Raytracer
