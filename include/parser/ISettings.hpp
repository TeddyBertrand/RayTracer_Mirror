#pragma once

#include <string>
#include <vector>
#include <memory>
#include "math/Vector3D.hpp" // Pour retourner directement des types utiles

namespace Raytracer
{
    class ISetting {
    public:
        virtual ~ISetting() = default;

        // Récupération de valeurs simples
        virtual int getInt(const std::string& path) const = 0;
        virtual double getFloat(const std::string& path) const = 0;
        virtual std::string getString(const std::string& path) const = 0;
        virtual bool getBool(const std::string& path) const = 0;

        // Récupération de types complexes (Helpers)
        virtual Vector3D getVector(const std::string& path) const = 0;
        
        // Navigation et vérification
        virtual bool exists(const std::string& path) const = 0;
        virtual std::shared_ptr<ISetting> getGroup(const std::string& path) const = 0;
    };
}
