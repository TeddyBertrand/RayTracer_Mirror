#include "MengerSpongeStrategy.hpp"

#include <algorithm>

namespace {

double centeredModulo(double value, double period) {
    return value - period * std::floor(value / period + 0.5);
}

} // namespace

namespace Raytracer {

FractalResult MengerSpongeStrategy::getInfo(const Vector3D& p, int maxIter) const {
    Vector3D currentP = p;
    double d = sdBox(currentP, {1.0, 1.0, 1.0});

    double s = 1.0;
    for (int i = 0; i < maxIter; i++) {
        Vector3D a(centeredModulo(currentP.x * s, 2.0),
                   centeredModulo(currentP.y * s, 2.0),
                   centeredModulo(currentP.z * s, 2.0));
        s *= 3.0;
        Vector3D r = (a * 3.0).abs();

        double da = std::max(r.x, r.y);
        double db = std::max(r.y, r.z);
        double dc = std::max(r.z, r.x);
        double c = (std::min(da, std::min(db, dc)) - 1.0) / s;

        d = std::max(d, -c);
    }

    return {d, 1.0};
}

double MengerSpongeStrategy::sdBox(Vector3D p, Vector3D b) const {
    Vector3D q = p.abs() - b;

    return Vector3D(std::max(q.x, 0.0), std::max(q.y, 0.0), std::max(q.z, 0.0)).length() +
           std::min(std::max(q.x, std::max(q.y, q.z)), 0.0);
}

} // namespace Raytracer
