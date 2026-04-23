#include "factory/PrimitiveFactory.hpp"
#include "Sphere.hpp"

namespace Raytracer
{

extern "C" void registerPlugin(PrimitiveFactory &factory)
{
    factory.registerType("sphere", [](const ISetting& settings) -> std::shared_ptr<IPrimitive>
    {
        float x = settings.getFloat("x");
        float y = settings.getFloat("y");
        float z = settings.getFloat("z");
        float radius = settings.getFloat("r");

        //add materials later

        return std::make_shared<Sphere>(Point3D{x, y, z}, radius, nullptr);
    });
}

bool solveQuadratic(const float &a, const float &b, const float &c, 
	float &x0, float &x1)
{
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

bool Sphere::hit(const Ray& r, Interval ray_t, HitRecord& rec) const
{
    Vector3D L = r.origin() - _center; // vector from center to ray origin
    float a = r.direction().dot(r.direction()); // direction lenght square
    float b = 2 * r.direction().dot(L); // ray/ sphere alignement
    float c = L.dot(L) - _radius * _radius; // start position
    
    float t0, t1;
    if (!solveQuadratic(a, b, c, t0, t1))
        return false;

    // root should be the nearest point
    float root = t0;
    if (root < ray_t.min || root > ray_t.max) {
        // if t0 is too far, try t1
        root = t1;
        if (root < ray_t.min || root > ray_t.max)
            return false; // neither worked
    }

    rec.t = root;
    rec.point = r.origin() + r.direction() * rec.t; //impact point
    
    // impact distance
    Vector3D normal = (rec.point - _center) / _radius;
    
    // check if ray hit inside or outside and oriente in function
    rec.setFaceNormal(r, normal);

    rec.material = _material;

    return true;
}

};
