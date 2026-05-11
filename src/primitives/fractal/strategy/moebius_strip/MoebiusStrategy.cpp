#include "MoebiusStrategy.hpp"
#include <algorithm>
#include <cmath>

namespace Raytracer {

FractalResult MoebiusStrategy::getInfo(const Vector3D& p, [[maybe_unused]] int maxIter) const {
    const double R = 1.0;
    const double w = 0.4;
    const double t = 0.05;

    double phi = std::atan2(p.z, p.x);

    double dist_h = std::sqrt(p.x * p.x + p.z * p.z) - R;
    double dist_v = p.y;

    double angle = phi * 0.5;
    double cosA = std::cos(angle);
    double sinA = std::sin(angle);

    double u = dist_h * cosA + dist_v * sinA;
    double v = -dist_h * sinA + dist_v * cosA;

    double dx = std::abs(u) - w;
    double dy = std::abs(v) - t;

    double outsideDist =
        std::sqrt(std::max(dx, 0.0) * std::max(dx, 0.0) + std::max(dy, 0.0) * std::max(dy, 0.0));
    double insideDist = std::min(std::max(dx, dy), 0.0);

    double finalDist = outsideDist + insideDist;

    return {finalDist, 0.0};
}

} // namespace Raytracer
