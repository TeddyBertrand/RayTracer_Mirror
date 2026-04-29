#pragma once

#include "math/Color.hpp"
#include "math/Vector3D.hpp"
#include <memory>
#include <string>

namespace Raytracer {

/**
 * @brief Interface de configuration en lecture seule.
 * Permet d'accéder aux données du fichier de scène de manière typée
 * et sécurisée via des chemins (ex: "camera.resolution.width").
 */
class ISetting {
public:
    virtual ~ISetting() = default;

    /** @brief Vérifie si un chemin existe dans la configuration. */
    virtual bool exists(const std::string& path) const = 0;

    /** @brief Récupère un entier. Lance une exception si absent. */
    virtual int getInt(const std::string& path) const = 0;

    /** @brief Récupère un entier avec valeur par défaut si absent. */
    virtual int getInt(const std::string& path, int defaultValue) const {
        return exists(path) ? getInt(path) : defaultValue;
    }

    /** @brief Récupère un nombre flottant (double). Lance une exception si absent. */
    virtual double getFloat(const std::string& path) const = 0;

    /** @brief Récupère un double avec valeur par défaut si absent. */
    virtual double getFloat(const std::string& path, double defaultValue) const {
        return exists(path) ? getFloat(path) : defaultValue;
    }

    /** @brief Récupère une chaîne de caractères. Lance une exception si absent. */
    virtual std::string getString(const std::string& path) const = 0;

    /** @brief Récupère une chaîne avec valeur par défaut (std::string). */
    virtual std::string getString(const std::string& path, const std::string& defaultValue) const {
        return exists(path) ? getString(path) : defaultValue;
    }

    /** @brief Récupère une chaîne avec valeur par défaut (const char*). */
    virtual std::string getString(const std::string& path, const char* defaultValue) const {
        return exists(path) ? getString(path) : std::string(defaultValue);
    }

    /** @brief Récupère un booléen. Lance une exception si absent. */
    virtual bool getBool(const std::string& path) const = 0;

    /** @brief Récupère un booléen avec valeur par défaut si absent. */
    virtual bool getBool(const std::string& path, bool defaultValue) const {
        return exists(path) ? getBool(path) : defaultValue;
    }

    /** @brief Récupère un Vector3D (ex: {x=1; y=2; z=3}). Lance une exception si absent. */
    virtual Vector3D getVector(const std::string& path) const = 0;

    /** @brief Récupère un Vector3D avec valeur par défaut si absent. */
    virtual Vector3D getVector(const std::string& path, const Vector3D& defaultValue) const {
        return exists(path) ? getVector(path) : defaultValue;
    }

    /** @brief Récupère une Color (ex: {r=255; g=0; b=0}). Lance une exception si absent. */
    virtual Color getColor(const std::string& path) const = 0;

    /** @brief Récupère une Color avec valeur par défaut si absent. */
    virtual Color getColor(const std::string& path, const Color& defaultValue) const {
        return exists(path) ? getColor(path) : defaultValue;
    }

    /** @brief Accède à un sous-groupe de configuration (ex: "camera.transform"). */
    virtual std::shared_ptr<ISetting> getGroup(const std::string& path) const = 0;
};

} // namespace Raytracer