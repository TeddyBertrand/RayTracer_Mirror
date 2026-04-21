#pragma once

#include <math/Vector3D.hpp>
#include <components/IPrimitive.hpp>

namespace Raytracer
{
    class Sphere : public virtual IPrimitive
    {
        public:
            Sphere(Point3D center, double radius) : _center(center), _radius(radius) {}
            Sphere() = default;

            bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const;

            std::string getName() const override { return "Sphere"; }

        private:
            Point3D _center;
            double _radius;
    };
};
