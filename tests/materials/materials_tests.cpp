#include <gtest/gtest.h>

#include "materials/lambertian/Lambertian.hpp"
#include "materials/transparent/Transparent.hpp"
#include "math/Color.hpp"
#include "math/HitRecord.hpp"
#include "math/Ray.hpp"
#include "math/Vector3D.hpp"

namespace {

void expectVectorNear(const Raytracer::Vector3D& actual, const Raytracer::Vector3D& expected,
                      double epsilon = 1e-6) {
    EXPECT_NEAR(actual.x, expected.x, epsilon);
    EXPECT_NEAR(actual.y, expected.y, epsilon);
    EXPECT_NEAR(actual.z, expected.z, epsilon);
}

} // namespace

// Lambertian tests
TEST(Lambertian, ScatterBasic) {
    Raytracer::Color albedo{0.8, 0.8, 0.8};
    Raytracer::Lambertian material(albedo);

    Raytracer::Ray ray_in({0.0, 0.0, -1.0}, {0.0, 0.0, 1.0});
    Raytracer::HitRecord rec;
    rec.point = {0.0, 0.0, 0.0};
    rec.normal = {0.0, 0.0, -1.0};
    rec.front_face = true;

    Raytracer::Color attenuation;
    Raytracer::Ray scattered;

    bool result = material.scatter(ray_in, rec, attenuation, scattered);

    EXPECT_TRUE(result);
    // Attenuation should be close to the albedo
    EXPECT_NEAR(attenuation.r, 0.8, 0.1);
    EXPECT_NEAR(attenuation.g, 0.8, 0.1);
    EXPECT_NEAR(attenuation.b, 0.8, 0.1);
    // Scattered ray should originate from hit point
    expectVectorNear(scattered.origin(), rec.point);
}

TEST(Lambertian, ScatterDifferentAlbedo) {
    Raytracer::Color albedo{0.2, 0.5, 0.9};
    Raytracer::Lambertian material(albedo);

    Raytracer::Ray ray_in({1.0, 1.0, -1.0}, {0.0, 0.0, 1.0});
    Raytracer::HitRecord rec;
    rec.point = {1.0, 1.0, 0.0};
    rec.normal = {0.0, 1.0, 0.0};
    rec.front_face = true;

    Raytracer::Color attenuation;
    Raytracer::Ray scattered;

    bool result = material.scatter(ray_in, rec, attenuation, scattered);

    EXPECT_TRUE(result);
    EXPECT_NEAR(attenuation.r, 0.2, 0.1);
    EXPECT_NEAR(attenuation.g, 0.5, 0.1);
    EXPECT_NEAR(attenuation.b, 0.9, 0.1);
}

TEST(Lambertian, ScatterBackFace) {
    Raytracer::Color albedo{0.8, 0.8, 0.8};
    Raytracer::Lambertian material(albedo);

    Raytracer::Ray ray_in({0.0, 0.0, 1.0}, {0.0, 0.0, -1.0});
    Raytracer::HitRecord rec;
    rec.point = {0.0, 0.0, 0.0};
    rec.normal = {0.0, 0.0, 1.0};
    rec.front_face = false;

    Raytracer::Color attenuation;
    Raytracer::Ray scattered;

    bool result = material.scatter(ray_in, rec, attenuation, scattered);

    EXPECT_TRUE(result);
    EXPECT_NEAR(attenuation.r, 0.8, 0.1);
}

TEST(Lambertian, GetName) {
    Raytracer::Lambertian material({0.8, 0.8, 0.8});

    EXPECT_EQ(material.getName(), "lambertian");
}

// Transparent tests
TEST(Transparent, BasicProperties) {
    Raytracer::Transparent material({0.9, 0.8, 0.7}, 1.5);

    EXPECT_EQ(material.getName(), "transparent");
    EXPECT_DOUBLE_EQ(material.getSpecularWeight(), 1.0);

    Raytracer::Color transmittance = material.getTransmittance();
    EXPECT_DOUBLE_EQ(transmittance.r, 0.9);
    EXPECT_DOUBLE_EQ(transmittance.g, 0.8);
    EXPECT_DOUBLE_EQ(transmittance.b, 0.7);
}

TEST(Transparent, ScatterUsesReflectionWhenCannotRefract) {
    Raytracer::Transparent material({0.7, 0.8, 0.9}, 1.5);

    Raytracer::Ray ray_in({0.0, 0.0, 0.0}, {0.8, 0.0, -0.6});
    Raytracer::HitRecord rec;
    rec.point = {1.0, 2.0, 3.0};
    rec.normal = {0.0, 0.0, 1.0};
    rec.front_face = false;

    Raytracer::Color attenuation;
    Raytracer::Ray scattered;

    bool result = material.scatter(ray_in, rec, attenuation, scattered);

    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(attenuation.r, 0.7);
    EXPECT_DOUBLE_EQ(attenuation.g, 0.8);
    EXPECT_DOUBLE_EQ(attenuation.b, 0.9);
    expectVectorNear(scattered.origin(), rec.point);

    Raytracer::Vector3D expected = {0.8, 0.0, 0.6};
    expectVectorNear(scattered.direction(), expected);
}
