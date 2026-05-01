#pragma once

#include "components/Entity.hpp"

namespace Raytracer {

class EntityBuilder
{
    public:

        EntityBuilder(std::string &name) : _name(name) {}

        EntityBuilder &setPrimitive(std::shared_ptr<IPrimitive> primitive) {
            _primitive = primitive;
            return *this;
        }
        
        EntityBuilder &addScale(double sx, double sy, double sz) {
            _transform = Matrix::scale(sx, sy, sz);
            return *this;
        }

        EntityBuilder &addTranslate(double x, double y, double z) {
            _transform = Matrix::translate(x, y, z);
            return *this;
        }

        EntityBuilder &addRotation(double x, double y, double z) {
            _transform = Matrix::rotateX(x);
            _transform = Matrix::rotateY(y);
            _transform = Matrix::rotateZ(z);
            return *this;
        }

        std::unique_ptr<Entity> build() {
            if (!_primitive)
                return NULL;
            std::unique_ptr<Entity> entity = std::make_unique<Entity>(_name, _primitive);
            entity->setTransform(_transform);

            return entity;
        }


    private:
        std::string _name;
        std::shared_ptr<IPrimitive> _primitive;
        std::shared_ptr<IMaterial> _material;
        Matrix _transform;
};

};
