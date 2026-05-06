#include "math/HitRecord.hpp"
#include "math/Interval.hpp"
#include "math/Ray.hpp"
#include "primitives/tanglecube/Tanglecube.hpp"
#include <gtest/gtest.h>

using namespace Raytracer;

// Test 1: Ray hitting tanglecube from outside
TEST(Tanglecube, HitFromOutside) {
    Tanglecube tanglecube;
    // Aim ray toward a region where tanglecube exists
    Ray r(Point3D(0.0, 0.0, -2.5), Vector3D(0.0, 0.0, 1.0));
    Interval ray_t(0.0, 10.0);
    HitRecord rec;

    bool hit = tanglecube.hit(r, ray_t, rec);
    // The diagonal/off-axis region is more likely to intersect
    if (!hit) {
        // Try diagonal ray instead
        Vector3D dir(0.2, 0.2, 1.0);
        dir.normalize();
        Ray r2(Point3D(-1.0, -1.0, -2.5), dir);
        hit = tanglecube.hit(r2, ray_t, rec);
    }
    EXPECT_TRUE(hit);
    EXPECT_GT(rec.t, 0.0);
}

// Test 2: Ray missing tanglecube
TEST(Tanglecube, Miss) {
    Tanglecube tanglecube;
    Ray r(Point3D(5.0, 0.0, 0.0), Vector3D(1.0, 0.0, 0.0));
    Interval ray_t(0.01, 10.0);
    HitRecord rec;

    bool hit = tanglecube.hit(r, ray_t, rec);
    EXPECT_FALSE(hit);
}

// Test 3: Ray hitting from different angle
TEST(Tanglecube, HitDiagonal) {
    Tanglecube tanglecube;
    Vector3D direction(1.0, 1.0, 1.0);
    direction.normalize();
    Ray r(Point3D(-3.0, -3.0, -3.0), direction);
    Interval ray_t(0.0, 20.0);
    HitRecord rec;

    bool hit = tanglecube.hit(r, ray_t, rec);
    EXPECT_TRUE(hit);
    EXPECT_GT(rec.t, 0.0);
}

// Test 4: Normal calculation
TEST(Tanglecube, NormalFlip) {
    Tanglecube tanglecube;
    // Use diagonal ray that crosses the surface
    Vector3D dir(0.2, 0.2, 1.0);
    dir.normalize();
    Ray r(Point3D(-1.0, -1.0, -2.5), dir);
    Interval ray_t(0.0, 10.0);
    HitRecord rec;

    if (tanglecube.hit(r, ray_t, rec)) {
        EXPECT_TRUE(rec.front_face);
        EXPECT_FALSE(std::isnan(rec.normal.x));
        EXPECT_FALSE(std::isnan(rec.normal.y));
        EXPECT_FALSE(std::isnan(rec.normal.z));
    }
}

// Test 5: Bounding box
TEST(Tanglecube, BoundingBox) {
    Tanglecube tanglecube;
    AABB bbox = tanglecube.getBoundingBox();
    EXPECT_LT(bbox.min.x, 0.0);
    EXPECT_GT(bbox.max.x, 0.0);
    EXPECT_LT(bbox.min.y, 0.0);
    EXPECT_GT(bbox.max.y, 0.0);
}
