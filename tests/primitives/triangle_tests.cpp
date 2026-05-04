#include <gtest/gtest.h>

#include "math/Interval.hpp"
#include "math/Ray.hpp"
#include "primitives/triangle/Triangle.hpp"

namespace {

void expectVectorNear(const Raytracer::Vector3D& actual,
                      const Raytracer::Vector3D& expected,
                      double epsilon = 1e-12) {
    EXPECT_NEAR(actual.x, expected.x, epsilon);
    EXPECT_NEAR(actual.y, expected.y, epsilon);
    EXPECT_NEAR(actual.z, expected.z, epsilon);
}

} // namespace

TEST(Triangle, RayHitsFromFront) {
    Raytracer::Triangle triangle({-1.0, -1.0, 0.0}, {1.0, -1.0, 0.0}, {0.0, 1.0, 0.0});
    Raytracer::HitRecord rec;

    // Ray coming from z=-3 towards +z
    const bool hit = triangle.hit({{0.0, 0.0, -3.0}, {0.0, 0.0, 1.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 3.0, 1e-6);
    EXPECT_TRUE(rec.front_face); // if normal is towards ray
    expectVectorNear(rec.point, {0.0, 0.0, 0.0});
}

TEST(Triangle, RayMissesOutsideEdges) {
    Raytracer::Triangle triangle({-1.0, -1.0, 0.0}, {1.0, -1.0, 0.0}, {0.0, 1.0, 0.0});
    Raytracer::HitRecord rec;

    // Ray misses to the right
    bool hit = triangle.hit({{2.0, 0.0, -3.0}, {0.0, 0.0, 1.0}}, {0.001, 100.0}, rec);
    EXPECT_FALSE(hit);

    // Ray misses up
    hit = triangle.hit({{0.0, 2.0, -3.0}, {0.0, 0.0, 1.0}}, {0.001, 100.0}, rec);
    EXPECT_FALSE(hit);
}

TEST(Triangle, RayOutsideInterval) {
    Raytracer::Triangle triangle({-1.0, -1.0, 0.0}, {1.0, -1.0, 0.0}, {0.0, 1.0, 0.0});
    Raytracer::HitRecord rec;

    // Ray would hit at t=3, but interval is [5, 10]
    const bool hit = triangle.hit({{0.0, 0.0, -3.0}, {0.0, 0.0, 1.0}}, {5.0, 10.0}, rec);

    EXPECT_FALSE(hit);
}

TEST(Triangle, BoundingBox) {
    Raytracer::Triangle triangle({0.0, 0.0, 0.0}, {2.0, 0.0, 0.0}, {0.0, 2.0, 0.0});

    Raytracer::AABB bbox = triangle.getBoundingBox();

    expectVectorNear(bbox.min, {0.0, 0.0, 0.0}, 1e-3);
    expectVectorNear(bbox.max, {2.0, 2.0, 0.0}, 1e-3);
}
