#pragma once

#include "components/Entity.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

class EntityBuilder
{
    public:

        /**
         * @brief Construct a new Entity Builder object
         * 
         * @param name 
         */
        EntityBuilder(std::string &name) : _name(name) {}

        /**
         * @brief Set the Primitive object
         * 
         * @param primitive 
         * @return EntityBuilder& 
         */
        EntityBuilder &setPrimitive(std::shared_ptr<IPrimitive> primitive) {
            _primitive = primitive;
            return *this;
        }

        /**
         * @brief Parse translation
         * 
         * apply translation to _transform with settings
         * @param settings 
         * @return EntityBuilder& 
         */
        EntityBuilder &parseTranslation(ISetting &settings) {
            Vector3D position = settings.getVector("position");

            _transform = Matrix::translate(position.x, position.y, position.z);
            return *this;
        }
        
        /**
         * @brief Parse scale
         * 
         * apply scale to _transform with settings
         * @param settings 
         * @return EntityBuilder& 
         */
        EntityBuilder &parseScale(ISetting &settings) {
            
            if (settings.exists("radius")) {
                float radius = settings.getFloat("radius");
                _transform = Matrix::scale(radius, radius, radius);

            } else {
                Vector3D scale = settings.getVector("scale");
                _transform = Matrix::scale(scale.x, scale.y, scale.z);
            }
            return *this;
        }

        /**
         * @brief Parse rotation
         * 
         * apply rotation to _transform with settings
         * @param settings 
         * @return EntityBuilder& 
         */
        EntityBuilder &parseRotation(ISetting &settings) {
            Vector3D rotation = settings.getVector("rotation");

            _transform = Matrix::rotateX(rotation.x);
            _transform = Matrix::rotateY(rotation.y);
            _transform = Matrix::rotateZ(rotation.z);
            return *this;
        }

        /**
         * @brief Add scale
         * 
         * Apply scale with raw values
         * @param sx 
         * @param sy 
         * @param sz 
         * @return EntityBuilder& 
         */
        EntityBuilder &addScale(double sx, double sy, double sz) {
            _transform = Matrix::scale(sx, sy, sz);
            return *this;
        }

        /**
         * @brief Add translation
         * 
         * @param x 
         * @param y 
         * @param z 
         * @return EntityBuilder& 
         */
        EntityBuilder &addTranslation(double x, double y, double z) {
            _transform = Matrix::translate(x, y, z);
            return *this;
        }

        /**
         * @brief Add rotation
         * 
         * @param x 
         * @param y 
         * @param z 
         * @return EntityBuilder& 
         */
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
