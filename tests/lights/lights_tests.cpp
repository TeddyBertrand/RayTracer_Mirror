#include <gtest/gtest.h>

#include "lights/point_light/PointLight.hpp"
#include "math/Color.hpp"
#include "math/Vector3D.hpp"

namespace {

void expectVectorNear(const Raytracer::Vector3D& actual, const Raytracer::Vector3D& expected,
                      double epsilon = 1e-6) {
    EXPECT_NEAR(actual.x, expected.x, epsilon);
    EXPECT_NEAR(actual.y, expected.y, epsilon);
    EXPECT_NEAR(actual.z, expected.z, epsilon);
}

} // namespace

// PointLight tests
TEST(PointLight, ComputeLightBasic) {
    Raytracer::PointLight light({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 1.0);
    Raytracer::Vector3D hit_point{1.0, 0.0, 0.0};

    Raytracer::LightSample sample = light.computeLight(hit_point);

    EXPECT_TRUE(sample.isActive);
    expectVectorNear(sample.direction, {-1.0, 0.0, 0.0});
    EXPECT_NEAR(sample.distance, 1.0, 1e-6);
    EXPECT_NEAR(sample.color.r, 1.0, 1e-6);
}

TEST(PointLight, ComputeLightWithDistance) {
    Raytracer::PointLight light({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 4.0);
    Raytracer::Vector3D hit_point{3.0, 4.0, 0.0};

    Raytracer::LightSample sample = light.computeLight(hit_point);

    EXPECT_TRUE(sample.isActive);
    EXPECT_NEAR(sample.distance, 5.0, 1e-6); // sqrt(9 + 16) = 5
    // attenuation = 4.0 / (5.0 * 5.0) = 0.16
    EXPECT_NEAR(sample.color.r, 0.16, 1e-6);
}

TEST(PointLight, ComputeLightNearSource) {
    Raytracer::PointLight light({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 1.0);
    Raytracer::Vector3D hit_point{0.1, 0.0, 0.0};

    Raytracer::LightSample sample = light.computeLight(hit_point);

    EXPECT_TRUE(sample.isActive);
    EXPECT_NEAR(sample.distance, 0.1, 1e-6);
}
