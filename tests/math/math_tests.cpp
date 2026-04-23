#include <gtest/gtest.h>

#include <cmath>

#include "math/Color.hpp"
#include "math/Interval.hpp"
#include "math/MathUtils.hpp"
#include "math/Ray.hpp"
#include "math/Vector3D.hpp"

namespace
{

void expectVectorNear(const Raytracer::Vector3D& actual,
                      const Raytracer::Vector3D& expected,
                      double epsilon = 1e-12)
{
    EXPECT_NEAR(actual.x, expected.x, epsilon);
    EXPECT_NEAR(actual.y, expected.y, epsilon);
    EXPECT_NEAR(actual.z, expected.z, epsilon);
}

} // namespace

TEST(Vector3D, BasicArithmetic)
{
    const Raytracer::Vector3D a{1.0, 2.0, 3.0};
    const Raytracer::Vector3D b{4.0, -2.0, 0.5};

    expectVectorNear(a + b, {5.0, 0.0, 3.5});
    expectVectorNear(a - b, {-3.0, 4.0, 2.5});
    expectVectorNear(2.0 * a, {2.0, 4.0, 6.0});
    expectVectorNear(a / 2.0, {0.5, 1.0, 1.5});
}

TEST(Vector3D, DotCrossAndNormalize)
{
    const Raytracer::Vector3D x = Raytracer::Vector3D::unitX();
    const Raytracer::Vector3D y = Raytracer::Vector3D::unitY();
    const Raytracer::Vector3D z = Raytracer::Vector3D::unitZ();

    EXPECT_DOUBLE_EQ(x.dot(y), 0.0);
    expectVectorNear(x.cross(y), z);

    const Raytracer::Vector3D normalized = Raytracer::Vector3D{0.0, 3.0, 4.0}.normalized();
    expectVectorNear(normalized, {0.0, 0.6, 0.8});
    EXPECT_NEAR(normalized.length(), 1.0, 1e-12);
}

TEST(Vector3D, NormalizeInPlace)
{
    Raytracer::Vector3D v{0.0, 0.0, 2.0};

    v.normalize();

    expectVectorNear(v, {0.0, 0.0, 1.0});
}

TEST(Vector3D, NearZero)
{
    const Raytracer::Vector3D small{1e-10, -1e-11, 9e-10};
    const Raytracer::Vector3D large{1e-4, 0.0, 0.0};

    EXPECT_TRUE(small.isNearZero());
    EXPECT_FALSE(large.isNearZero());
}

TEST(Ray, At)
{
    const Raytracer::Ray ray({1.0, 2.0, 3.0}, {2.0, 0.0, -1.0});

    expectVectorNear(ray.at(2.5), {6.0, 2.0, 0.5});
}

TEST(Interval, ContainsAndClamp)
{
    const Raytracer::Interval interval{-1.0, 1.0};

    EXPECT_TRUE(interval.contains(1.0));
    EXPECT_TRUE(interval.surrounds(0.0));
    EXPECT_FALSE(interval.surrounds(-1.0));
    EXPECT_DOUBLE_EQ(interval.clamp(-2.0), -1.0);
    EXPECT_DOUBLE_EQ(interval.clamp(0.5), 0.5);
    EXPECT_DOUBLE_EQ(interval.clamp(3.0), 1.0);
}

TEST(Color, ToByte)
{
    EXPECT_EQ(Raytracer::Color::toByte(0.0), 0);
    EXPECT_EQ(Raytracer::Color::toByte(0.25), 128);
    EXPECT_EQ(Raytracer::Color::toByte(1.0), 255);
}

TEST(MathUtils, DegreesAndRadians)
{
    const double pi = std::acos(-1.0);

    EXPECT_NEAR(Raytracer::Math::degreesToRadians(180.0), pi, 1e-12);
    EXPECT_NEAR(Raytracer::Math::radiansToDegrees(pi), 180.0, 1e-12);
}

// Vector3D augmented assignment operators
TEST(Vector3D, CompoundAssignment)
{
    Raytracer::Vector3D v{1.0, 2.0, 3.0};
    Raytracer::Vector3D a{2.0, 3.0, 4.0};

    // Test +=
    v += a;
    expectVectorNear(v, {3.0, 5.0, 7.0});

    // Test *= (scalar)
    v *= 2.0;
    expectVectorNear(v, {6.0, 10.0, 14.0});

    // Test /=
    v /= 2.0;
    expectVectorNear(v, {3.0, 5.0, 7.0});
}

// Vector3D unary operators and other free functions
TEST(Vector3D, UnaryOperatorAndMultiplication)
{
    Raytracer::Vector3D v{1.0, -2.0, 3.0};

    // Test unary negation
    expectVectorNear(-v, {-1.0, 2.0, -3.0});

    // Test scalar multiplication from both sides
    expectVectorNear(v * 2.0, {2.0, -4.0, 6.0});
    expectVectorNear(3.0 * v, {3.0, -6.0, 9.0});
}

// Vector3D division operator
TEST(Vector3D, DivisionOperator)
{
    Raytracer::Vector3D v{6.0, 4.0, 2.0};

    expectVectorNear(v / 2.0, {3.0, 2.0, 1.0});
}

// Vector3D equality operator
TEST(Vector3D, EqualityOperator)
{
    Raytracer::Vector3D a{1.0, 2.0, 3.0};
    Raytracer::Vector3D b{1.0, 2.0, 3.0};
    Raytracer::Vector3D c{1.0, 2.0, 3.1};

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

// Vector3D static factory methods  
TEST(Vector3D, StaticFactoryMethods)
{
    expectVectorNear(Raytracer::Vector3D::up(), {0.0, 1.0, 0.0});
    expectVectorNear(Raytracer::Vector3D::down(), {0.0, -1.0, 0.0});
    expectVectorNear(Raytracer::Vector3D::left(), {-1.0, 0.0, 0.0});
    expectVectorNear(Raytracer::Vector3D::right(), {1.0, 0.0, 0.0});
    expectVectorNear(Raytracer::Vector3D::forward(), {0.0, 0.0, -1.0});
    expectVectorNear(Raytracer::Vector3D::back(), {0.0, 0.0, 1.0});
}

// Vector3D random methods
TEST(Vector3D, RandomVector)
{
    Raytracer::Vector3D v = Raytracer::Vector3D::random();
    
    // Components should be in range [-1, 1)
    EXPECT_GE(v.x, -1.0);
    EXPECT_LT(v.x, 1.0);
    EXPECT_GE(v.y, -1.0);
    EXPECT_LT(v.y, 1.0);
    EXPECT_GE(v.z, -1.0);
    EXPECT_LT(v.z, 1.0);
}

// Vector3D random unit vector
TEST(Vector3D, RandomUnitVector)
{
    Raytracer::Vector3D v = Raytracer::Vector3D::getRandomUnitVector();
    
    // Length should be approximately 1
    EXPECT_NEAR(v.length(), 1.0, 1e-6);
}

// Vector3D normalized with zero vector
TEST(Vector3D, NormalizedZeroVector)
{
    Raytracer::Vector3D zero{0.0, 0.0, 0.0};
    Raytracer::Vector3D normalized = zero.normalized();
    
    expectVectorNear(normalized, {0.0, 0.0, 0.0});
}

// Color operators
TEST(Color, ColorOperators)
{
    Raytracer::Color c1{0.5, 0.3, 0.2};
    Raytracer::Color c2{0.1, 0.2, 0.3};

    // Test +=
    c1 += c2;
    EXPECT_NEAR(c1.r, 0.6, 1e-12);
    EXPECT_NEAR(c1.g, 0.5, 1e-12);
    EXPECT_NEAR(c1.b, 0.5, 1e-12);

    // Test *= (scalar)
    Raytracer::Color c3{0.5, 0.5, 0.5};
    c3 *= 2.0;
    EXPECT_NEAR(c3.r, 1.0, 1e-12);
    EXPECT_NEAR(c3.g, 1.0, 1e-12);
    EXPECT_NEAR(c3.b, 1.0, 1e-12);

    // Test *= (color)
    Raytracer::Color c4{0.5, 0.5, 0.5};
    Raytracer::Color c5{0.4, 0.4, 0.4};
    c4 *= c5;
    EXPECT_NEAR(c4.r, 0.2, 1e-12);
    EXPECT_NEAR(c4.g, 0.2, 1e-12);
    EXPECT_NEAR(c4.b, 0.2, 1e-12);
}

// Color free functions
TEST(Color, ColorFreeFunctions)
{
    Raytracer::Color c1{0.5, 0.3, 0.2};
    Raytracer::Color c2{0.1, 0.2, 0.3};

    // Test operator+
    Raytracer::Color sum = c1 + c2;
    EXPECT_NEAR(sum.r, 0.6, 1e-12);
    EXPECT_NEAR(sum.g, 0.5, 1e-12);
    EXPECT_NEAR(sum.b, 0.5, 1e-12);

    // Test operator* (color * color)
    Raytracer::Color c3{0.5, 0.5, 0.5};
    Raytracer::Color c4{0.4, 0.4, 0.4};
    Raytracer::Color prod = c3 * c4;
    EXPECT_NEAR(prod.r, 0.2, 1e-12);
    EXPECT_NEAR(prod.g, 0.2, 1e-12);
    EXPECT_NEAR(prod.b, 0.2, 1e-12);

    // Test scalar multiplication
    Raytracer::Color scaled = 2.0 * c1;
    EXPECT_NEAR(scaled.r, 1.0, 1e-12);
    EXPECT_NEAR(scaled.g, 0.6, 1e-12);
    EXPECT_NEAR(scaled.b, 0.4, 1e-12);

    // Test scalar multiplication (right side)
    scaled = c1 * 2.0;
    EXPECT_NEAR(scaled.r, 1.0, 1e-12);
    EXPECT_NEAR(scaled.g, 0.6, 1e-12);
    EXPECT_NEAR(scaled.b, 0.4, 1e-12);

    // Test division by scalar
    Raytracer::Color c5{4.0, 6.0, 8.0};
    Raytracer::Color div = c5 / 2.0;
    EXPECT_NEAR(div.r, 2.0, 1e-12);
    EXPECT_NEAR(div.g, 3.0, 1e-12);
    EXPECT_NEAR(div.b, 4.0, 1e-12);
}

// Color default constructor
TEST(Color, DefaultConstructor)
{
    Raytracer::Color c;
    EXPECT_NEAR(c.r, 0.0, 1e-12);
    EXPECT_NEAR(c.g, 0.0, 1e-12);
    EXPECT_NEAR(c.b, 0.0, 1e-12);
}
