#pragma once

#include <math/Vector3D.hpp>
#include <components/IPrimitive.hpp>

namespace Raytracer
{
    class Sphere : public virtual IPrimitive
    {
        public:
            /**
             * @brief Construct a new Sphere object with params
             * 
             * @param center 
             * @param radius 
             */
            Sphere(Point3D center, double radius) : _center(center), _radius(radius) {}

            /**
             * @brief Hit function
             * 
             * check if a ray between an interval t0 and t1 has hit the sphere or not 
             * @param r 
             * @param ray_t 
             * @param rec 
             * @return true 
             * @return false 
             */
            bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const;

        private:
            Point3D _center;
            double _radius;
    };
};
