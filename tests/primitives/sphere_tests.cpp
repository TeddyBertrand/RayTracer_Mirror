#include <gtest/gtest.h>

#include "math/Interval.hpp"
#include "math/Ray.hpp"
#include "primitives/sphere/Sphere.hpp"

namespace {

void expectVectorNear(const Raytracer::Vector3D& actual,
                      const Raytracer::Vector3D& expected,
                      double epsilon = 1e-12) {
    EXPECT_NEAR(actual.x, expected.x, epsilon);
    EXPECT_NEAR(actual.y, expected.y, epsilon);
    EXPECT_NEAR(actual.z, expected.z, epsilon);
}

} // namespace

TEST(Sphere, RayHitsFromOutside) {
    Raytracer::Sphere sphere({0.0, 0.0, 0.0}, 1.0, nullptr);
    Raytracer::HitRecord rec;

    const bool hit = sphere.hit({{0.0, 0.0, -3.0}, {0.0, 0.0, 1.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 2.0, 1e-6);
    EXPECT_TRUE(rec.front_face);
    expectVectorNear(rec.point, {0.0, 0.0, -1.0});
    expectVectorNear(rec.normal, {0.0, 0.0, -1.0});
}

TEST(Sphere, RayMisses) {
    Raytracer::Sphere sphere({0.0, 0.0, 0.0}, 1.0, nullptr);
    Raytracer::HitRecord rec;

    const bool hit = sphere.hit({{0.0, 0.0, -3.0}, {0.0, 1.0, 0.0}}, {0.001, 100.0}, rec);

    EXPECT_FALSE(hit);
}

TEST(Sphere, RayFromInsideHitsAndFlipsNormal) {
    Raytracer::Sphere sphere({0.0, 0.0, 0.0}, 1.0, nullptr);
    Raytracer::HitRecord rec;

    const bool hit = sphere.hit({{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 1.0, 1e-6);
    EXPECT_FALSE(rec.front_face);
    expectVectorNear(rec.point, {0.0, 0.0, 1.0});
    expectVectorNear(rec.normal, {0.0, 0.0, -1.0});
}

TEST(Sphere, RayOutsideInterval) {
    Raytracer::Sphere sphere({0.0, 0.0, 0.0}, 1.0, nullptr);
    Raytracer::HitRecord rec;

    // Ray does hit sphere but intersection is outside the interval
    const bool hit = sphere.hit({{0.0, 0.0, -3.0}, {0.0, 0.0, 1.0}}, {10.0, 100.0}, rec);

    EXPECT_FALSE(hit);
}

TEST(Sphere, RayPartiallyInInterval) {
    Raytracer::Sphere sphere({5.0, 0.0, 0.0}, 1.0, nullptr);
    Raytracer::HitRecord rec;

    // Ray hits sphere, but test with narrow interval that includes one intersection
    const bool hit = sphere.hit({{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}}, {2.0, 5.5}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 4.0, 1e-6);
}

TEST(Sphere, RayHitsAtTwoPoints) {
    Raytracer::Sphere sphere({0.0, 0.0, 0.0}, 2.0, nullptr);
    Raytracer::HitRecord rec;

    // Ray intersection with two points in valid range
    const bool hit = sphere.hit({{0.0, 0.0, -5.0}, {0.0, 0.0, 1.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 3.0, 1e-6);
}
