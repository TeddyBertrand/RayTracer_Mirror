# Adding a New Material Plugin

Materials define how a surface interacts with light. In this engine, materials are modular and use a **BSDF** (Bidirectional Scattering Distribution Function) to handle the physics of light scattering.

## 1. The Header File (`TemplateMaterial.hpp`)

Your new material must inherit from `IMaterial`. Since the engine uses a BSDF-based approach, the primary responsibility of the material class is to provide access to its BSDF logic.

```cpp
#pragma once

#include <memory>
#include "components/IMaterial.hpp"
#include "components/IBSDF.hpp"
#include "components/ITexture.hpp"

namespace Raytracer {

    class TemplateMaterial : public IMaterial {
    public:
        /**
         * @brief Constructor
         * @param tex The texture to be used (color, image, etc.)
         * @param randomness Degree of scattering (0.0 for smooth, 1.0 for rough)
         * @param custom_bsdf Optional custom BSDF logic (e.g., for normal mapping)
         */
        TemplateMaterial(std::shared_ptr<ITexture> tex,
                         double randomness = 1.0,
                         std::shared_ptr<IBSDF> custom_bsdf = nullptr);

        /**
         * @brief Returns the BSDF associated with this material.
         * The engine calls this to calculate how light bounces off the surface.
         */
        const IBSDF& getBSDF() const override { return *_bsdf; }

    private:
        double _randomness;
        std::shared_ptr<IBSDF> _bsdf;
    };

} // namespace Raytracer
```

## 2. The Implementation File (`TemplateMaterial.cpp`)

The implementation file contains the logic to instantiate your material and the entry point for the plugin system. It uses the `ITexture` and `IBSDF` components to define surface behavior.

```cpp
#include "TemplateMaterial.hpp"
#include "materials/commun/texture/Texture.hpp"
#include "materials/commun/bsdf/normal_mapping/NormalMappingBSDF.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

extern "C" {
    /**
     * @brief Returns the unique identifier for this material plugin.
     * This string must match the 'type' field in your .config file.
     */
    const char* getName() { return "flat_color"; }

    /**
     * @brief Plugin Factory Function.
     * Extracts configuration data and returns a new Material instance.
     */
    IMaterial* createPlugin(const ISetting& settings) {

        // 1. Extract basic parameters with default values
        double randomness = settings.getFloat("randomness", 0.0);
        
        // 2. Load the main texture (can be a solid color or an image)
        std::shared_ptr<ITexture> tex = Texture::fromSetting(settings, "color");

        // 3. Handle optional Normal Mapping
        std::shared_ptr<IBSDF> bsdf;
        if (settings.exists("normal")) {
            // Create the base reflection model (Lambertian)
            auto base_bsdf = std::make_shared<LambertianBSDF>(tex, randomness);
            
            // Load the normal map texture and its strength
            std::shared_ptr<ITexture> normal_tex = Texture::fromSetting(settings, "normal");
            double strength = settings.getFloat("normal_strength", 1.0);
            
            // Wrap the base BSDF with Normal Mapping logic
            bsdf = std::make_shared<NormalMappingBSDF>(base_bsdf, normal_tex, strength);
        }

        return new TemplateMaterial(tex, randomness, bsdf);
    }
}

/**
 * @brief Constructor implementation.
 * Ensures parameters like randomness are clamped and initializes the BSDF.
 */
TemplateMaterial::TemplateMaterial(std::shared_ptr<ITexture> tex,
                                   double randomness,
                                   std::shared_ptr<IBSDF> custom_bsdf)
    : _randomness(randomness < 0.0 ? 0.0 : (randomness > 1.0 ? 1.0 : randomness)) {
    
    // If a special BSDF (like Normal Mapping) was provided, use it.
    // Otherwise, default to a standard Lambertian reflection.
    if (custom_bsdf) {
        _bsdf = custom_bsdf;
    } else {
        _bsdf = std::make_shared<LambertianBSDF>(tex, _randomness);
    }
}

} // namespace Raytracer
```

## 3. The Global Directory (`src/materials/CMakeLists.txt`)

This file manages the collection of all material plugins. It fetch sfml,  defines the common output path (where the `.so` files will be sent) and includes each material's individual directory.

```cmake
include(FetchContent)

set(SFML_STATIC_LIBRARIES OFF CACHE BOOL "" FORCE)
set(RAYTRACER_BUILD_SHARED_LIBS_BACKUP ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS ON)

find_package(SFML 3 QUIET COMPONENTS Graphics System)
if(NOT SFML_FOUND)
    message(STATUS "SFML not found, fetching it now")
    FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
    )
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_BACKUP ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/_deps/lib)
    FetchContent_MakeAvailable(sfml)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_BACKUP})
endif()

set(BUILD_SHARED_LIBS ${RAYTRACER_BUILD_SHARED_LIBS_BACKUP})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins/materials)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins/materials)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/plugins/materials)

# add the new material directory name here :
add_subdirectory(commun)
add_subdirectory(flat_material)
add_subdirectory(light_material)
add_subdirectory(metal_material)
add_subdirectory(transparent_material)
add_subdirectory(phong_material)
add_subdirectory(template_material)
```

## 4. The Local Plugin (`src/materials/[your_material]/CMakeLists.txt`)

This file compiles your material's source code into a shared library. It ensures that the plugin is linked with the necessary BSDF logic and external libraries like SFML.

```cmake
# We use 'raytracer_templatematerial' as a generic name for this guide
add_library(raytracer_templatematerial SHARED
    TemplateMaterial.cpp # add your .cpp here
)

# Materials require 'raytracer_materials_common' (for BSDFs) 
# and SFML (to handle textures and images).
if(TARGET sfml-graphics)
    target_link_libraries(raytracer_templatematerial PRIVATE 
        raytracer_materials_common 
        sfml-graphics 
        sfml-system
    )
elseif(TARGET SFML::Graphics)
    target_link_libraries(raytracer_templatematerial PRIVATE 
        raytracer_materials_common 
        SFML::Graphics 
        SFML::System
    )
else()
    target_link_libraries(raytracer_templatematerial PRIVATE 
        raytracer_materials_common 
        ${SFML_LIBRARIES}
    )
endif()

target_include_directories(raytracer_templatematerial PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_templatematerial PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_templatematerial PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_templatematerial"
)
```

## 5. The Material Directory Structure

### A. Source Structure (`src/materials/`)
This is where you develop your code. Each material has its own folder containing its logic and its local build rules.

```text
src/
└── materials/
    ├── CMakeLists.txt           # Global manager (add_subdirectory)
    ├── commun/                  # Shared BSDFs and Texture logic
    └── template_material/       # Your specific material folder
        ├── CMakeLists.txt       # Local build rules
        ├── TemplateMaterial.hpp # Header
        └── TemplateMaterial.cpp # Implementation
```
