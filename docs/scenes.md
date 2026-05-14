# 🎬 Scene Configuration Guide

This guide explains how to structure your `.scene` files for the Raytracer. We use the **libconfig** format, which allows for a hierarchical and modular organization. Each section corresponds to a specific component of your 3D universe.

---

## 1. The Camera

The camera is the central and mandatory element. It defines your point of view in space and the resolution of the final image.

### Concepts

*   **"perspective" Type**: Simulates human vision. Objects recede toward a vanishing point, creating natural depth.
*   **Resolution (width/height)**: Image size in pixels (e.g., 1280x720). This defines the number of primary rays cast by the engine.
*   **Position**: The $\{x, y, z\}$ coordinates of the observer.
*   **Look_at**: The target point the camera is looking at.
*   **Up**: The vector defining the vertical direction (usually $\{y = 1.0\}$).
*   **Field of View (FOV)**: The opening angle. A small value zooms in, while a large value provides a wide-angle view.

### Configuration

```c
camera:
{
    type = "perspective";
    width = 1280;
    height = 720;
    
    # Observer's location
    position = { x = 0.0; y = 2.0; z = 10.0; };
    
    # Target point
    look_at = { x = 0.0; y = 0.0; z = 0.0; };
    
    # Vertical orientation
    up = { x = 0.0; y = 1.0; z = 0.0; };
    
    # Viewing angle in degrees
    fieldOfView = 50.0;
};
```

## 2. Materials Library

The `materials` block is your scene's palette. It allows you to define the visual "DNA" of an object separately from its geometric shape. By using a centralized library, you can apply the same complex look to multiple objects instantly.

### Concepts

*   **The ID System**: Every material must have a unique `id` string. In the `shapes` block, you will call this ID to dress your object. This separation of concerns makes your scene files much easier to manage.
*   **The 'type' Field**: This is the most critical field. It tells the engine which specific material plugin to wake up.
    *   `flat_color`: Simple, uniform color without complex lighting calculations.
    *   `phong`: A classic model for shiny surfaces with highlights.
    *   `reflective`: Used for mirrors or chrome-like surfaces.
    * etc ...
*   **Color Values**: In this engine, colors are typically defined in the RGB scale from `0.0` to `255.0`. 
*   **Surface Properties**: 
    *   **randomness**: Adds "noise" to the surface, useful for creating matte or rough finishes.

### Configuration

```c
materials: (
    { 
        id = "floor_white"; 
        type = "flat_color"; 
        color = { r = 200.0; g = 200.0; b = 200.0; }; 
    },
    { 
        id = "bright_blue"; 
        type = "flat_color"; 
        color = { r = 0.0; g = 100.0; b = 255.0; }; 
        randomness = 0.2; # Subtle roughness
    }
);
```

## 3. Shapes and Primitives

The `shapes` block is where you populate your 3D world. While the Camera defines *where* you look from, and Materials define *how* things look, Shapes define *what* is actually there. Each shape is a geometric object powered by a specific intersection plugin.

### Concepts

*   **Geometric Plugins**: Every shape has a `type` (e.g., "sphere", "box", "plane"). This corresponds to a specific algorithm that calculates if a ray of light hits that object.
*   **Material Assignment**: The `material` field must contain an `id` that matches one of the materials defined in your **Materials Library**. If the ID doesn't exist, the engine will typically apply a default "error" texture (like a purple/black checkerboard).
*   **Transformations**:
    *   **Position**: The center point of the object in 3D space $\{x, y, z\}$.
    *   **Rotation**: The orientation of the object, usually defined in degrees for the X, Y, and Z axes. Note that for symmetrical objects like spheres, rotation might not be visually apparent unless a texture is applied.
    *   **Scale**: How large or small the object is.
    *   **Shear**: A transformation that slants the shape by shifting coordinates along one axis based on the value of another (e.g., tilting a box sideways).
*   **Specific Parameters**: Depending on the `type`, shapes require unique variables. For example, a `sphere` needs a `radius`, while a `cone` might need a `height` and a `base`.

### Configuration

```c
shapes: (
    { 
        type = "plane"; 
        position = { x = 0.0; y = -1.5; z = 0.0; }; 
        material = "floor_white"; 
    },
    { 
        type = "sphere"; 
        position = { x = 0.0; y = 0.0; z = 0.0; }; 
        radius = 1.5; 
        material = "bright_blue"; 
    },
    {
        type = "box";
        position = { x = -3.0; y = 0.0; z = -2.0; };
        rotation = { x = 0.0; y = 45.0; z = 0.0 };
        size = { x = 1.0; y = 1.0; z = 1.0 };
        material = "floor_white";
    }
    {
        type = "box";
        position = { x = 2.0; y = 0.0; z = 0.0 };
        material = "bright_blue";
        shear = { xy = 0.5; xz = 0.0; yx = 0.0; yz = 0.0; zx = 0.0; zy = 0.0; };
    }
);
```

## 4. Lights

The `lights` block defines the light sources in your scene. Lights interact with materials to create highlights, diffuse colors, and shadows.

### Concepts

*   **Point Light**: Acts like a tiny lightbulb. It exists at a specific point in space and emits light in all directions. Its intensity usually fades (attenuates) with distance.
*   **Directional Light**: Acts like the Sun. It has no specific position, only a direction. All light rays are parallel, meaning shadows will also be parallel.
*   **Ambient Light**: A low-level constant light that hits every object equally. This prevents "true black" shadows and simulates light bouncing off walls.
*   **Shadows**: In a raytracer, shadows are created by "Shadow Rays." For every point the camera sees, the engine shoots a ray toward each light source. If an object is in the way, that point is in shadow.

### Configuration

```c
lights: (
    { 
        type = "point"; 
        position = { x = 10.0; y = 10.0; z = 10.0; }; 
        color = { r = 255.0; g = 255.0; b = 255.0; }; 
        intensity = 1.0; 
    },
    { 
        type = "directional"; 
        direction = { x = -1.0; y = -1.0; z = -1.0; }; 
        color = { r = 255.0; g = 240.0; b = 200.0; }; # Warm sunlight
        intensity = 0.8; 
    },
    {
        type = "ambient";
        color = { r = 25.0; g = 25.0; b = 25.0; };
        intensity = 0.1;
    }
);
```

## 5. Sky Configuration

The `sky` block defines what the rays hit when they don't intersect any geometry. Instead of a simple black background, you can use specialized plugins to create environments.

### Concepts

*   **Background Fallback**: When a ray misses all shapes in the scene, the engine queries the Sky plugin to determine the pixel color.
*   **Atmospheric**: Simulates a simple gradient, usually between the horizon and the zenith (the point directly above). Great for outdoor scenes.
*   **Empty**: The simplest plugin. It typically returns a solid color (black), representing the void of space or a dark room.
*   **Galaxy**: A procedural plugin that generates stars and nebulae. It’s perfect for space-themed renders without needing high-resolution textures.

### Configuration

You can choose **one** of the following types for your sky block:

```c
# Example 1: Atmospheric Sky
sky:
{
    type = "atmospheric";
    groundColor = { r = 127.0; g = 178.0; b = 255.0; };
    zenithColor = { r = 255.0; g = 255.0; b = 255.0; };
};

# Example 2: Galaxy Sky
sky:
{
    type = "galaxy";
    starDensity = 0.002;         # Probability of a star appearing in a pixel
    nebulaColor = { r = 10.0; g = 5.0; b = 20.0; };
    nebulaDensity = 0.8;         # How much space the nebula clouds occupy
    nebulaContrast = 2.5;        # Sharpness of the nebula structures
};

# Example 3: Empty Sky
sky:
{
    type = "empty";
};
```


## 6. Graphics & Display (SFML Runtime)

The graphics block handles the visual output of your engine. Using the SFML plugin enables a "Live Display" window, allowing you to see the image being built in real-time rather than waiting for a file to save.

### Concepts

*   **Live Buffer**: The engine continuously updates a pixel buffer that SFML draws to the screen.
*   **Scale Factor**: The `scale` parameter is useful for high-resolution renders. A scale of 2 will make the window twice as small as the actual render resolution, ensuring it fits on your screen.
*   **Window Management**: This block also defines the title of the window and the window dimensions.

### Configuration
```c
graphics:
{
    type = "sfml";
    width = 800;
    height = 600;
    title = "RayTracer - Live Display";
    scale = 1; # 1 = original size, 2 = half size, etc.
};
```

---

## 7. Render & Preview Settings

To optimize your workflow, the engine distinguishes between a high-quality "Render" and a fast "Preview". This allows you to move the camera around quickly before committing to a long, high-quality calculation.

### Concepts

*   **Samples**: In `render`, high samples (e.g., 1000) eliminate noise. In `preview`, 1 sample provides instant feedback.
*   **Adaptive Threshold**: This optimization tells the engine to stop sampling a pixel if its color has stabilized, significantly speeding up the process.
*   **Ambient Occlusion (AO)**: This simulates soft shadows in corners. `ao_samples` defines the quality of these shadows.

### Configuration

```c
render:
{
    type = "default";
    samples = 1000;
    adaptive_threshold = 0.01;
    ao_samples = 32;
    ao_max_distance = 100.0;
};

preview:
{
    type = "fast";
    samples = 1;
    max_depth = 2;
};
```
