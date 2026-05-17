# 🟢 Adding a New Shape Plugin

Shapes (or Primitives) are the geometric objects of your raytracer. In this engine, a Shape is a mathematical plugin that defines its own intersection logic. 

**Note on Transformations:** You don't need to handle position, rotation, or scaling inside your Shape's `hit` logic. The engine uses an `Entity` decorator to handle these automatically. Your shape should ideally be defined at the origin $(0,0,0)$.

## 1. The Header File (`TemplateShape.hpp`)

This file defines the class for your new geometric primitive. It must inherit from `IPrimitive`.

```cpp
#pragma once

#include <memory>
#include <iostream>
#include "components/IPrimitive.hpp"
#include "math/Vector3D.hpp"
#include "math/HitRecord.hpp"
#include "math/Interval.hpp"
#include "math/AABB.hpp"

namespace Raytracer {

    class TemplateShape : public virtual IPrimitive {
    public:
        /**
         * @brief Default constructor. 
         * @param you could add as many paramters as you wish
         * Shapes are usually created at the origin with unit size.
         */
        TemplateShape(float height, float base_radius) 
        : _height(height), _radius(base_radius) {}

        /**
         * @brief Intersection logic.
         * @param r The ray in local space.
         * @param ray_t The valid distance interval.
         * @param rec The HitRecord to fill if an intersection occurs.
         */
        bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;

        /**
         * @brief Axis-Aligned Bounding Box.
         * Used for performance optimization (BVH).
         */
        AABB getBoundingBox() const override;

        /**
         * @brief Debug method to display the object type in the console.
         */
        void dump(int indent) const override;

    private:
        // Internal data
        double _height;
        double _radius;
    };

} // namespace Raytracer
```

## 2. The Implementation File (`TemplateShape.cpp`)

The implementation handles the actual math and the **Plugin Entry Point**. In this engine, we use an `EntityBuilder` to wrap our mathematical primitive with transformations and materials.

```cpp
#include "TemplateShape.hpp"
#include "builder/EntityBuilder.hpp"
#include "parser/ISettings.hpp"
#include <cmath>

namespace Raytracer {

    bool TemplateShape::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
        // --- YOUR MATH LOGIC HERE ---
        return false; 
    }

    AABB TemplateShape::getBoundingBox() const {
        // Return a box based on your custom parameters
        return AABB{Point3D(-_radius, 0, -_radius), Point3D(_radius, _height, _radius)};
    }

    void TemplateShape::dump(int indent) const {
        std::cout << std::string(indent, ' ') << "- [TemplateShape]" << std::endl;
    }

} // namespace Raytracer


extern "C" {
    const char* getName() { return "template_shape"; }

    /**
     * Factory function: This is the bridge between the .config file 
     * and your C++ code.
     */
    Raytracer::IPrimitive* createPlugin(const Raytracer::ISetting& settings) {
        // 1. Extract CUSTOM parameters from settings
        // These are specific to your shape
        float base_r = settings.getFloat("base_radius", 1.0f);
        float h = settings.getFloat("height", 1.0f);

        // 2. Instantiate your primitive with these custom arguments
        auto myShape = std::make_shared<Raytracer::TemplateShape>(h, base_r);

        // 3. Use EntityBuilder to handle standard Raytracer features
        // (Position, Rotation, Scale, Shear, Material)
        Raytracer::EntityBuilder builder(settings);

        std::unique_ptr<Raytracer::Entity> entity =
            builder.setPrimitive(myShape)
                   .parseTransform(settings)
                   .parseMaterial(settings)
                   .build();

        // 4. Return the built Entity
        return entity.release();
    }
}
```

## 3. The Build System (CMake)

To compile your shapes as dynamic plugins, you need to configure two levels of `CMakeLists.txt`. This ensures they are moved to the `plugins/primitives` folder where the engine can find them.

### A. The Global Directory (`src/primitives/CMakeLists.txt`)

This file acts as the manager for all your shapes. It defines the common output directory and includes each shape's folder.

```cmake
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins/primitives)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins/primitives)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/plugins/primitives)

# Add each shape's subdirectory
# When you create a new shape, register its folder here:
add_subdirectory(sphere)
add_subdirectory(plane)
add_subdirectory(limited_cone)
# add_subdirectory(template_shape)
```

### B. The Local Plugin (`src/primitives/[your_shape]/CMakeLists.txt`)

Each specific shape needs its own `CMakeLists.txt`. This file tells CMake how to compile your C++ code into a Shared Library (.so) that the Raytracer can load at runtime.

```cmake
# Replace 'raytracer_template_shape' with your shape's name (ex: raytracer_sphere)
add_library(raytracer_template_shape SHARED
    TemplateShape.cpp # add your .cpp file
)

target_include_directories(raytracer_template_shape PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(raytracer_template_shape PRIVATE
    -W
    -Wall
    -Wextra
)

set_target_properties(raytracer_template_shape PROPERTIES
    PREFIX ""
    OUTPUT_NAME "raytracer_template_shape"
)
```

## 5. The Plugin Directory Structure

Just like lights, each shape must be contained within its own folder inside `src/primitives/`. This ensures that each geometric object (Sphere, Plane, etc.) is independent and easy to manage.

**Example Structure:**
```text
src/primitives/
├── CMakeLists.txt           (The Parent file)
└── template_shape/          (Specific folder for your shape)
    ├── CMakeLists.txt       (The Plugin file)
    ├── TemplateShape.hpp    (Class declaration)
    └── TemplateShape.cpp    (Logic & Plugin entry point)
```