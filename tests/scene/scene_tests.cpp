#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "components/ILight.hpp"
#include "components/IPrimitive.hpp"
#include "core/scene/Scene.hpp"

namespace
{

class FakePrimitive : public Raytracer::IPrimitive
{
public:
    FakePrimitive(bool shouldHit, double t) : _shouldHit(shouldHit), _t(t) {}

    bool hit(const Raytracer::Ray&, Raytracer::Interval, Raytracer::HitRecord& rec) const override
    {
        if (!_shouldHit) {
            return false;
        }
        rec.t = _t;
        rec.point = {0.0, 0.0, _t};
        return true;
    }

    std::string getName() const override
    {
        return "FakePrimitive";
    }

private:
    bool _shouldHit;
    double _t;
};

class FakeLight : public Raytracer::ILight
{
public:
    Raytracer::LightSample computeLight(const Raytracer::Point3D&) const override
    {
        return {{1.0, 1.0, 1.0}, Raytracer::Vector3D::unitZ(), 1.0, true};
    }
};

} // namespace

TEST(Scene, AddedLightIsExposedByGetter)
{
    Raytracer::Scene scene;

    scene.addLight(std::make_shared<FakeLight>());

    ASSERT_EQ(scene.getLights().size(), 1U);
}

TEST(Scene, AddedPrimitiveCanBeHitThroughWorld)
{
    Raytracer::Scene scene;
    Raytracer::HitRecord rec;

    scene.addPrimitive(std::make_shared<FakePrimitive>(true, 3.0));
    const bool hit = scene.getWorld().hit({{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_DOUBLE_EQ(rec.t, 3.0);
}

TEST(PrimitiveList, ReturnsClosestHit)
{
    Raytracer::PrimitiveList list;
    Raytracer::HitRecord rec;

    list.add(std::make_shared<FakePrimitive>(true, 5.0));
    list.add(std::make_shared<FakePrimitive>(true, 2.0));

    const bool hit = list.hit({{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}}, {0.001, 100.0}, rec);

    EXPECT_TRUE(hit);
    EXPECT_DOUBLE_EQ(rec.t, 2.0);
}

TEST(PrimitiveList, ReturnsFalseWhenEmpty)
{
    Raytracer::PrimitiveList list;
    Raytracer::HitRecord rec;

    const bool hit = list.hit({{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}}, {0.001, 100.0}, rec);

    EXPECT_FALSE(hit);
}
