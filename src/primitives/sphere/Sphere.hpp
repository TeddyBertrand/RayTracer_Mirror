#pragma once

#include <memory>

#include <math/Vector3D.hpp>
#include <components/IPrimitive.hpp>
#include "components/IMaterial.hpp"

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
             * @param material 
             */
            Sphere(Point3D center, double radius, std::shared_ptr<IMaterial> material)
                : _center(center), _radius(radius), _material(material) {}

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

            std::string getName() const override { return "Sphere"; }

        private:
            Point3D _center;
            double _radius;
            std::shared_ptr<IMaterial> _material;
    };
};
