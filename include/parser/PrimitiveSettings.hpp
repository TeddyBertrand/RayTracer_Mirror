#pragma once

#include "components/IMaterial.hpp"
#include "parser/ISettings.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace Raytracer {

/**
 * @brief Wrapper pour les primitives permettant de lier les IDs de matériaux
 * aux instances réelles de IMaterial tout en déléguant le reste à ISetting.
 */
class PrimitiveSetting : public ISetting {
public:
    PrimitiveSetting(const ISetting& base,
                     const std::unordered_map<std::string, std::shared_ptr<IMaterial>>& materials)
        : _base(base), _materials(materials) {}

    virtual ~PrimitiveSetting() = default;

    /** @brief Relaye la vérification d'existence à la base. */
    bool exists(const std::string& path) const override { return _base.exists(path); }

    // --- Int ---
    int getInt(const std::string& path) const override { return _base.getInt(path); }
    int getInt(const std::string& path, int defaultValue) const override {
        return _base.getInt(path, defaultValue);
    }

    // --- Float ---
    double getFloat(const std::string& path) const override { return _base.getFloat(path); }
    double getFloat(const std::string& path, double defaultValue) const override {
        return _base.getFloat(path, defaultValue);
    }

    // --- String ---
    std::string getString(const std::string& path) const override { return _base.getString(path); }
    std::string getString(const std::string& path, const std::string& defaultValue) const override {
        return _base.getString(path, defaultValue);
    }
    std::string getString(const std::string& path, const char* defaultValue) const override {
        return _base.getString(path, defaultValue);
    }

    // --- Bool ---
    bool getBool(const std::string& path) const override { return _base.getBool(path); }
    bool getBool(const std::string& path, bool defaultValue) const override {
        return _base.getBool(path, defaultValue);
    }

    // --- Vector3D ---
    Vector3D getVector(const std::string& path) const override { return _base.getVector(path); }
    Vector3D getVector(const std::string& path, const Vector3D& defaultValue) const override {
        return _base.getVector(path, defaultValue);
    }

    // --- Color ---
    Color getColor(const std::string& path) const override { return _base.getColor(path); }
    Color getColor(const std::string& path, const Color& defaultValue) const override {
        return _base.getColor(path, defaultValue);
    }

    // --- Group ---
    std::shared_ptr<ISetting> getGroup(const std::string& path) const override {
        return _base.getGroup(path);
    }

    /**
     * @brief Récupère le matériau associé à la primitive.
     * @return shared_ptr vers IMaterial ou nullptr si non trouvé/défini.
     */
    std::shared_ptr<IMaterial> getMaterial() const {
        if (!exists("material"))
            return nullptr;
        std::string id = getString("material");
        auto it = _materials.find(id);
        return (it != _materials.end()) ? it->second : nullptr;
    }

private:
    const ISetting& _base;
    const std::unordered_map<std::string, std::shared_ptr<IMaterial>>& _materials;
};

} // namespace Raytracer
