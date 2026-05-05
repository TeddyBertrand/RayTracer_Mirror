#include <gtest/gtest.h>

#include "components/Entity.hpp"
#include "math/Interval.hpp"
#include "math/Ray.hpp"
#include "primitives/cylinder/Cylinder.hpp"

namespace {

void expectVectorNear(const Raytracer::Vector3D& actual,
                      const Raytracer::Vector3D& expected,
                      double epsilon = 1e-12) {
    EXPECT_NEAR(actual.x, expected.x, epsilon);
    EXPECT_NEAR(actual.y, expected.y, epsilon);
    EXPECT_NEAR(actual.z, expected.z, epsilon);
}

} // namespace

TEST(Cylinder, RayHitsFromOutside) {
    Raytracer::Cylinder cylinder;
    Raytracer::HitRecord rec;

    // Ray from outside, hitting the cylinder side
    const bool hit = cylinder.hit({{3.0, 1.0, 0.0}, {-1.0, 0.0, 0.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 2.0, 1e-6);
    expectVectorNear(rec.point, {1.0, 1.0, 0.0});
    // Normal should point outward in the XZ plane (no Y component)
    expectVectorNear(rec.normal, {1.0, 0.0, 0.0});
    EXPECT_TRUE(rec.front_face);
}

TEST(Cylinder, RayMissesAbove) {
    Raytracer::Cylinder cylinder;
    Raytracer::HitRecord rec;

    // Ray above the cylinder (outside height bounds)
    const bool hit = cylinder.hit({{0.0, 5.0, 0.0}, {0.0, 0.0, 1.0}}, {0.001, 100.0}, rec);

    EXPECT_FALSE(hit);
}

TEST(Cylinder, RayMissesRadially) {
    Raytracer::Cylinder cylinder;
    Raytracer::HitRecord rec;

    // Ray that passes beyond the cylinder radius
    const bool hit = cylinder.hit({{3.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}, {0.001, 100.0}, rec);

    EXPECT_FALSE(hit);
}

TEST(Cylinder, RayFromInsideHitsAndFlipsNormal) {
    Raytracer::Cylinder cylinder;
    Raytracer::HitRecord rec;

    // Ray from inside the cylinder
    const bool hit = cylinder.hit({{0.0, 1.0, 0.0}, {1.0, 0.0, 0.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 1.0, 1e-6);
    expectVectorNear(rec.point, {1.0, 1.0, 0.0});
    // Normal should be flipped (pointing inward)
    expectVectorNear(rec.normal, {-1.0, 0.0, 0.0});
    EXPECT_FALSE(rec.front_face);
}

TEST(Cylinder, RayHitsBothSides) {
    Raytracer::Cylinder cylinder;
    Raytracer::HitRecord rec;

    // Ray passing through cylinder - should hit the near side first
    const bool hit = cylinder.hit({{-3.0, 1.0, 0.0}, {1.0, 0.0, 0.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 2.0, 1e-6);
    expectVectorNear(rec.point, {-1.0, 1.0, 0.0});
    expectVectorNear(rec.normal, {-1.0, 0.0, 0.0});
    EXPECT_TRUE(rec.front_face);
}

TEST(Cylinder, RayOutsideHeightBoundsButWithinRadius) {
    Raytracer::Cylinder cylinder;
    Raytracer::HitRecord rec;

    // Ray at correct radius but below the cylinder
    const bool hit = cylinder.hit({{0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}}, {0.001, 100.0}, rec);

    EXPECT_FALSE(hit);
}

TEST(Cylinder, RayHitsBottomCap) {
    Raytracer::Cylinder cylinder;
    Raytracer::HitRecord rec;

    const bool hit = cylinder.hit({{0.0, -1.0, 0.0}, {0.0, 1.0, 0.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 1.0, 1e-6);
    expectVectorNear(rec.point, {0.0, 0.0, 0.0});
    expectVectorNear(rec.normal, {0.0, -1.0, 0.0});
    EXPECT_TRUE(rec.front_face);
}

TEST(Cylinder, RayHitsTopCap) {
    Raytracer::Cylinder cylinder;
    Raytracer::HitRecord rec;

    const bool hit = cylinder.hit({{0.0, 2.0, 0.0}, {0.0, -1.0, 0.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 1.0, 1e-6);
    expectVectorNear(rec.point, {0.0, 1.0, 0.0});
    expectVectorNear(rec.normal, {0.0, 1.0, 0.0});
    EXPECT_TRUE(rec.front_face);
}

TEST(Cylinder, RayFromInsideHitsTopCapAndFlipsNormal) {
    Raytracer::Cylinder cylinder;
    Raytracer::HitRecord rec;

    const bool hit = cylinder.hit({{0.0, 0.5, 0.0}, {0.0, 1.0, 0.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 0.5, 1e-6);
    expectVectorNear(rec.point, {0.0, 1.0, 0.0});
    expectVectorNear(rec.normal, {0.0, -1.0, 0.0});
    EXPECT_FALSE(rec.front_face);
}

TEST(Cylinder, TransformedUnitCylinderStillHitsViaEntity) {
    auto cylinder = std::make_shared<Raytracer::Cylinder>();
    auto entity = std::make_shared<Raytracer::Entity>("UnitCylinder", cylinder);

    entity->scale(2.0, 2.0, 2.0);
    entity->translate(0.0, 1.0, 0.0);

    Raytracer::HitRecord rec;
    const bool hit = entity->hit({{0.0, 4.0, 0.0}, {0.0, -1.0, 0.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 1.0, 1e-6);
    expectVectorNear(rec.point, {0.0, 3.0, 0.0});
    expectVectorNear(rec.normal, {0.0, 1.0, 0.0});
    EXPECT_TRUE(rec.front_face);
}
