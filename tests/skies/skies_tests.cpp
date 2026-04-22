#include <gtest/gtest.h>

#include "math/Color.hpp"
#include "math/Ray.hpp"
#include "skies/atmospheric_sky/AtmosphericSky.hpp"
#include "skies/empty_sky/EmptySky.hpp"
#include "skies/galaxy_sky/GalaxySky.hpp"

namespace
{

void expectColorNear(const Raytracer::Color& actual,
                     const Raytracer::Color& expected,
                     double epsilon = 1e-12)
{
    EXPECT_NEAR(actual.r, expected.r, epsilon);
    EXPECT_NEAR(actual.g, expected.g, epsilon);
    EXPECT_NEAR(actual.b, expected.b, epsilon);
}

} // namespace

TEST(EmptySky, ReturnsBlackBackground)
{
    Raytracer::EmptySky sky;
    const Raytracer::Ray ray({0.0, 0.0, 0.0}, {1.0, 0.0, 0.0});

    expectColorNear(sky.getBackgroundColor(ray), {0.0, 0.0, 0.0});
    expectColorNear(sky.getEnvironmentColor(ray), {0.0, 0.0, 0.0});
    EXPECT_EQ(sky.getName(), "empty_sky");
}

TEST(AtmosphericSky, InterpolatesBetweenGroundAndZenith)
{
    const Raytracer::Color ground{0.5, 0.7, 1.0};
    const Raytracer::Color zenith{1.0, 1.0, 1.0};
    Raytracer::AtmosphericSky sky(ground, zenith);

    EXPECT_EQ(sky.getName(), "atmospheric_sky");

    const Raytracer::Ray up_ray({0.0, 0.0, 0.0}, {0.0, 1.0, 0.0});
    const Raytracer::Ray down_ray({0.0, 0.0, 0.0}, {0.0, -1.0, 0.0});
    const Raytracer::Ray horizon_ray({0.0, 0.0, 0.0}, {1.0, 0.0, 0.0});

    expectColorNear(sky.getBackgroundColor(up_ray), zenith);
    expectColorNear(sky.getEnvironmentColor(up_ray), zenith);
    expectColorNear(sky.getBackgroundColor(down_ray), ground);
    expectColorNear(sky.getEnvironmentColor(down_ray), ground);
    expectColorNear(sky.getBackgroundColor(horizon_ray), {0.75, 0.85, 1.0});
}

TEST(GalaxySky, ZeroStarAndNebulaDensityGivesBlackBackground)
{
    const Raytracer::Color nebulaColor{0.2, 0.4, 0.6};
    Raytracer::GalaxySky sky(0.0, nebulaColor, 0.0, 3.0);
    const Raytracer::Ray ray({0.0, 0.0, 0.0}, {0.3, 0.4, 0.5});

    EXPECT_EQ(sky.getName(), "galaxy_sky");
    expectColorNear(sky.getBackgroundColor(ray), {0.0, 0.0, 0.0});
    expectColorNear(sky.getEnvironmentColor(ray), nebulaColor * 0.8);
}

TEST(GalaxySky, BackgroundColorIsDeterministic)
{
    Raytracer::GalaxySky sky(0.001, {0.025, 0.01, 0.05}, 0.7, 3.0);
    const Raytracer::Ray ray({0.0, 0.0, 0.0}, {0.2, 0.8, -0.4});

    const Raytracer::Color first = sky.getBackgroundColor(ray);
    const Raytracer::Color second = sky.getBackgroundColor(ray);

    expectColorNear(first, second);
}
