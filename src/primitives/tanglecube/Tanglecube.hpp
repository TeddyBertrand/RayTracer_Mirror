#pragma once

#include <memory>

#include "components/IPrimitive.hpp"
#include "math/Vector3D.hpp"

namespace Raytracer {

/**
 * @brief Tanglecube implicit surface primitive.
 * Implements the classic tanglecube equation:
 * x^4 + y^4 + z^4 - 5(x^2 + y^2 + z^2) + 11.8 = 0
 * Unit-ish local space centered at the origin.
 * Use Entity scale to adjust size; transformed via Entity for position/rotation.
 */
class Tanglecube : public virtual IPrimitive {
public:
    Tanglecube() : _center(0.0, 0.0, 0.0), _scale(1.0) {}

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;

    AABB getBoundingBox() const override {
        // Conservative bound for the quartic implicit surface.
        // The maximum extent is sqrt((5 + sqrt(27.8)) / 2) ≈ 2.26.
        Point3D halfExtent(_scale * 2.5, _scale * 2.5, _scale * 2.5);
        return AABB{_center - halfExtent, _center + halfExtent};
    }

    void dump(int indent) const override {
        std::cout << std::string(indent, ' ') << "- \033[1;33m[Tanglecube]\033[0m" << std::endl;
    }

private:
    /**
     * @brief Evaluate the tanglecube implicit function f(p) = 0
     * @param p Point to evaluate
     * @return Function value (sign indicates inside/outside)
     */
    double implicitFunction(const Vector3D& p) const;

    /**
     * @brief Compute gradient of the tanglecube at point p for normal estimation
     * @param p Point where gradient is evaluated
     * @param grad Output gradient vector
     */
    void computeGradient(const Vector3D& p, Vector3D& grad) const;

    Point3D _center;
    double _scale;
};

} // namespace Raytracer
