#include <gtest/gtest.h>

#include "core/camera/PerspectiveCamera.hpp"
#include "core/image/Image.hpp"
#include "math/Vector3D.hpp"
#include "math/Ray.hpp"

namespace
{

void expectVectorNear(const Raytracer::Vector3D& actual,
                      const Raytracer::Vector3D& expected,
                      double epsilon = 1e-6)
{
    EXPECT_NEAR(actual.x, expected.x, epsilon);
    EXPECT_NEAR(actual.y, expected.y, epsilon);
    EXPECT_NEAR(actual.z, expected.z, epsilon);
}

} // namespace

// PerspectiveCamera tests
TEST(PerspectiveCamera, RayGeneration)
{
    Raytracer::PerspectiveCamera camera(
        {0.0, 0.0, 0.0},    // origin
        {0.0, 0.0, -1.0},   // rotation
        90.0,                // fov
        1.0,                 // aspect_ratio
        800,                 // width
        600                  // height
    );

    // Get ray at center of viewport
    Raytracer::Ray ray = camera.getRay(0.5, 0.5);

    // Ray should originate from camera position
    expectVectorNear(ray.origin(), {0.0, 0.0, 0.0});
}

TEST(PerspectiveCamera, RayAtCorner)
{
    Raytracer::PerspectiveCamera camera(
        {0.0, 0.0, 0.0},
        {0.0, 0.0, -1.0},
        90.0,
        1.0,
        640,
        480
    );

    Raytracer::Ray ray = camera.getRay(0.0, 0.0);
    expectVectorNear(ray.origin(), {0.0, 0.0, 0.0});
}

TEST(PerspectiveCamera, RayAtOtherCorner)
{
    Raytracer::PerspectiveCamera camera(
        {0.0, 0.0, 0.0},
        {0.0, 0.0, -1.0},
        90.0,
        1.0,
        640,
        480
    );

    Raytracer::Ray ray = camera.getRay(1.0, 1.0);
    expectVectorNear(ray.origin(), {0.0, 0.0, 0.0});
}

TEST(PerspectiveCamera, DifferentPosition)
{
    Raytracer::PerspectiveCamera camera(
        {5.0, 5.0, 5.0},
        {-1.0, -1.0, -1.0},
        60.0,
        16.0 / 9.0,
        1920,
        1080
    );

    Raytracer::Ray ray = camera.getRay(0.5, 0.5);
    expectVectorNear(ray.origin(), {5.0, 5.0, 5.0});
}

// Image tests
TEST(Image, Construction)
{
    Raytracer::Image image(10, 20);
    
    // Verify the image was constructed (no crash)
    // Private members, so we can't verify directly
}

TEST(Image, ConstructionDifferentSize)
{
    Raytracer::Image image(1920, 1080);
    
    // Verify it constructs without error
}

TEST(Image, ConstructionSmall)
{
    Raytracer::Image image(1, 1);
    
    // Verify it constructs without error
}
