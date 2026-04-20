#include "Sphere.hpp"

namespace Raytracer
{

bool solveQuadratic(const float &a, const float &b, const float &c, 
	float &x0, float &x1)
{
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
    if (x0 > x1)
        std::swap(x0, x1);
    
    return true;
}

bool Sphere::hit(const Ray& r, Interval ray_t, HitRecord& rec) const
{
    Vector3D L = r.origin() - _center;
    float a = r.direction().dot(r.direction());
    float b = 2 * r.direction().dot(L);
    float c = L.dot(L) - _radius * _radius;
    
    float t0, t1;
    if (!solveQuadratic(a, b, c, t0, t1))
        return false;

    float root = t0;
    if (root < ray_t.min || root > ray_t.max) {
        root = t1;
        if (root < ray_t.min || root > ray_t.max)
            return false;
    }

    rec.t = root;
    rec.point = r.origin() + r.direction() * rec.t;
    
    Vector3D normal = (rec.point - _center) / _radius;
    
    rec.set_face_normal(r, normal);

    return true;
}

};
