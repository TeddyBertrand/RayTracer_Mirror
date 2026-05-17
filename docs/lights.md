# 💡 Adding a New Light Plugin

This guide explains how to create a new light source plugin. Lights are loaded dynamically, allowing you to add new types of illumination without recompiling the main engine.

## 1. The Header File (`TemplateLight.hpp`)

In the header, you define the data your light needs. 

**Note on the Constructor:** You can add as many arguments as you want to your constructor (position, radius, falloff, etc.). You simply need to ensure that you extract these corresponding values from the `ISetting` object inside the `createPlugin` function later.

```cpp
#pragma once

#include "components/ILight.hpp"
#include "math/Color.hpp"
#include "math/Vector3D.hpp"


namespace Raytracer {
    class TemplateLight : public ILight {
    public:
        /**
         * @brief Custom Constructor
         * @param color Base color of the light
         * @param intensity Brightness multiplier
         * 
         * Feel free to add more parameters here to fit your light's needs.
         */
        TemplateLight(const Color& color, double intensity)
            : _color(color), _intensity(intensity) {}

        /**
         * @brief Calculates lighting contribution using the HitRecord.
         * 
         * Provides access to advanced shading data such as surface normals, 
         * hit point coordinates, and texture mapping (UVs).
         * 
         * @param hit The HitRecord containing all information about the intersection.
         * @return LightSample containing direction, distance, and emitted color.
         */
        LightSample computeLight(const HitRecord& hit) const override;

    private:
        Color _color;
        double _intensity;
    };
}
```

## 2. The Implementation File (`TemplateLight.cpp`)

This file contains the logic for calculating the light vector and the plugin entry points for the engine.

```cpp
#include "TemplateLight.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

    LightSample TemplateLight::computeLight(const HitRecord& hit) const {
        LightSample sample;
        
        // You have access to the full HitRecord:
        // hit.point  -> The 3D position of the impact
        // hit.normal -> The surface normal at the impact
        
        // Example for a simple Directional Light logic:
        sample.direction = Vector3D(0, 1, 0); // Direction TOWARDS the light
        sample.distance = 1e9;                // Infinite distance for global lights
        sample.color = _color * _intensity;   // Final resulting color
        sample.isActive = true;               // Is the light affecting this point?

        return sample;
    }

} // namespace Raytracer

extern "C" {
    // This name is used in the .config file: type = "template_light";
    const char* getName() { return "template_light"; }

    /**
     * Factory function: This is where the magic happens.
     * You read values from the config file and pass them to your constructor.
     */
    Raytracer::ILight* createPlugin(const Raytracer::ISetting& settings) {
        // 1. Extract values using the ISettings helper

        // No fallback value provided: this parameter must be defined in the configuration.
        Raytracer::Color color = settings.getColor("color");

        // Fallback value provided: if this parameter is not defined in the configuration, take 1.0 instead
        double intensity = settings.getFloat("intensity", 1.0); 
        
        // Return a new instance of your custom light
        return new Raytracer::TemplateLight(color, intensity);
    }
}
```

## 3. The Build System (CMake)

To compile your lights, you need to configure two `CMakeLists.txt` files. This ensures that each light is built as a standalone plugin and moved to the correct directory.

### A. The Parent Directory (`src/lights/CMakeLists.txt`)

This file manages the organization of all light plugins. It defines where the compiled files will be stored and which subdirectories to include.

```cmake
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins/lights)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins/lights)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/plugins/lights)

# Every time you create a new light folder, add it here:
add_subdirectory(point_light)
add_subdirectory(directional_light)
add_subdirectory(spot_light)
# add_subdirectory(template_light)
```

### B. The Local Plugin (`src/lights/[your_light]/CMakeLists.txt`)

```cmake
# Replace 'raytracer_template_light' with your light's name (ex: raytracer_point_light)
add_library(raytracer_template_light SHARED
    TemplateLight.cpp # add your .cpp file
)

target_include_directories(raytracer_template_light PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_template_light PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_template_light PROPERTIES
    PREFIX ""
    OUTPUT_NAME "template_light" 
)
```

### 5. The Plugin Directory Structure

Each light must be contained within its own folder inside `src/lights/`. This isolation is what allows the modular system to scale.

**Example Structure:**
```text
src/lights/
├── CMakeLists.txt (The Parent file)
└── template_light/
    ├── CMakeLists.txt (The Plugin file)
    ├── TemplateLight.hpp
    └── TemplateLight.cpp
```