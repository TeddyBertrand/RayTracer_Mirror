#include "Cylinder.hpp"
#include "factory/PrimitiveFactory.hpp"
#include "materials/lambertian/Lambertian.hpp"
#include "parser/PrimitiveSettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "cylinder"; }

IPrimitive* createPlugin(const ISetting& settings) {
    Vector3D pos = settings.getVector("position");
    double radius = settings.getFloat("radius");
    double height = settings.getFloat("height");

    const auto* pSettings = dynamic_cast<const PrimitiveSetting*>(&settings);

    std::shared_ptr<IMaterial> mat = nullptr;
    if (pSettings) {
        mat = pSettings->getMaterial();
    }

    return new Cylinder(pos, radius, height, mat);
}
}

bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1) {
    // get the discriminant
    float discr = b * b - 4 * a * c;
    if (discr < 0)
        return false;
    else if (discr == 0)
        x0 = x1 = -0.5 * b / a;
    else {
        float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    // make sure that x0 is still the smallest value
    if (x0 > x1)
        std::swap(x0, x1);

    return true;
}

bool Cylinder::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    // Calculate coefficients for ray-cylinder intersection
    // Cylinder equation: (x - cx)^2 + (z - cz)^2 = r^2 (aligned along Y axis)
    // Ray equation: P(t) = O + tD

    double dx = r.direction().x;
    double dz = r.direction().z;
    double ox = r.origin().x - _center.x;
    double oz = r.origin().z - _center.z;

    float a = dx * dx + dz * dz;
    float b = 2.0 * (dx * ox + dz * oz);
    float c = (ox * ox + oz * oz) - (_radius * _radius);

    float t0, t1;
    if (!solveQuadratic(a, b, c, t0, t1)) {
        return false;
    }

    // Find the nearest valid intersection
    float root = t0;
    if (root < ray_t.min || root > ray_t.max) {
        root = t1;
        if (root < ray_t.min || root > ray_t.max) {
            return false;
        }
    }

    // Check if the intersection point is within the cylinder's height bounds
    double y_hit = r.origin().y + root * r.direction().y;
    if (y_hit < _center.y || y_hit > _center.y + _height) {
        // Try the other root if the first was outside height bounds
        root = t1;
        y_hit = r.origin().y + root * r.direction().y;
        if (root < ray_t.min || root > ray_t.max || y_hit < _center.y ||
            y_hit > _center.y + _height) {
            return false;
        }
    }

    rec.t = root;
    rec.point = r.origin() + r.direction() * rec.t;

    // Calculate the outward normal (only depends on X and Z for cylinder aligned to Y)
    Vector3D outward_normal = Vector3D(rec.point.x - _center.x, 0, rec.point.z - _center.z);
    outward_normal.normalize();

    rec.setFaceNormal(r, outward_normal);
    rec.material = _material;

    return true;
}

}; // namespace Raytracer
