# Raytracer

A high-performance, plugin-based Raytracer built in C++20. This engine is designed for total extensibility, allowing users to dynamically load shapes, lights, and materials.

---

## Getting Started

### 1. Prerequisites

Ensure you have the following tools installed:
* **C++ Compiler**: Supporting C++20 (GCC 10+, Clang 10+, or MSVC 19.28+).
* **CMake**: Version 3.15 or higher.
* **Libraries**:
    * `libconfig++`: For parsing scene configuration files.

### 2. Quick Build

To create the build directory, configure the project, and compile everything (including plugins) in one go, run:

```bash
mkdir -p build && cmake -B build -S . && cmake --build build
```

### 3. Running a Scene

To render a scene, you simply need to pass a configuration file (`.scene`) as an argument to the executable. The engine will parse the file, load the necessary plugins, and start the rendering process.

```bash
./raytracer scenes/showcase/primitives/sphere.scene
```

## 📖 Documentation

To help you extend the engine, we have modularized the documentation. Follow these guides to add new features via our plugin system:

### 🧩 Plugin Development Guides
* [**Adding a Light**](./docs/lights.md) — Learn how to create new lights.
* [**Adding a Shape**](./docs/shapes.md) — Learn how to add your own shapes
* [**Adding a Material**](./docs/materials.md) — Learn how to create new materials

### 🎬 Scene Configuration
* [**Scene Setup**](./docs/scenes.md) — Detailed guide on how to configure your `.scene` files.