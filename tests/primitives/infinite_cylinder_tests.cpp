#include <gtest/gtest.h>

#include "components/Entity.hpp"
#include "math/Ray.hpp"
#include "primitives/infinite_cylinder/InfiniteCylinder.hpp"

namespace {

void expectVectorNear(const Raytracer::Vector3D& actual,
                      const Raytracer::Vector3D& expected,
                      double epsilon = 1e-12) {
    EXPECT_NEAR(actual.x, expected.x, epsilon);
    EXPECT_NEAR(actual.y, expected.y, epsilon);
    EXPECT_NEAR(actual.z, expected.z, epsilon);
}

} // namespace

TEST(InfiniteCylinder, RayHitsFromOutside) {
    Raytracer::InfiniteCylinder cyl;
    Raytracer::HitRecord rec;

    const bool hit = cyl.hit({{3.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 2.0, 1e-6);
    expectVectorNear(rec.point, {1.0, 0.0, 0.0});
    expectVectorNear(rec.normal, {1.0, 0.0, 0.0});
    EXPECT_TRUE(rec.front_face);
}

TEST(InfiniteCylinder, RayMissesRadially) {
    Raytracer::InfiniteCylinder cyl;
    Raytracer::HitRecord rec;

    const bool hit = cyl.hit({{3.0, 0.0, 0.0}, {0.0, 0.0, 1.0}}, {0.001, 100.0}, rec);

    EXPECT_FALSE(hit);
}

TEST(InfiniteCylinder, RayFromInsideHitsAndFlipsNormal) {
    Raytracer::InfiniteCylinder cyl;
    Raytracer::HitRecord rec;

    const bool hit = cyl.hit({{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 1.0, 1e-6);
    expectVectorNear(rec.point, {1.0, 0.0, 0.0});
    expectVectorNear(rec.normal, {-1.0, 0.0, 0.0});
    EXPECT_FALSE(rec.front_face);
}

TEST(InfiniteCylinder, TransformedUnitInfiniteCylinderStillHitsViaEntity) {
    auto prim = std::make_shared<Raytracer::InfiniteCylinder>();
    auto entity = std::make_shared<Raytracer::Entity>("InfCyl", prim);

    // Scale radius by 2 (x and z), leaving Y unchanged
    entity->scale(2.0, 1.0, 2.0);

    Raytracer::HitRecord rec;
    // Ray along -Z hitting at z=2 (since scaled radius = 2)
    const bool hit = entity->hit({{0.0, 0.0, 4.0}, {0.0, 0.0, -1.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 2.0, 1e-6);
    expectVectorNear(rec.point, {0.0, 0.0, 2.0});
    expectVectorNear(rec.normal, {0.0, 0.0, 1.0});
    EXPECT_TRUE(rec.front_face);
}
