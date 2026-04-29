#pragma once

#include "components/IMaterial.hpp"
#include "parser/ISettings.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace Raytracer {

/**
 * @class PrimitiveSetting
 * @brief Decorator for ISetting that handles material resolution for primitives.
 * * This class wraps a base ISetting object and provides specialized logic to map material
 * identifiers (strings) found in the configuration to actual IMaterial instances.
 * All standard configuration calls are delegated to the underlying base setting.
 */
class PrimitiveSetting : public ISetting {
public:
    /**
     * @brief Constructs a PrimitiveSetting wrapper.
     * @param base Reference to the underlying configuration group for this primitive.
     * @param materials Map of available material instances indexed by their unique identifiers.
     */
    PrimitiveSetting(const ISetting& base,
                     const std::unordered_map<std::string, std::shared_ptr<IMaterial>>& materials)
        : _base(base), _materials(materials) {}

    virtual ~PrimitiveSetting() = default;

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
     * @brief Resolves and retrieves the material associated with this primitive.
     * * This method looks for a "material" key in the configuration, retrieves the
     * identifier string, and performs a lookup in the provided material map.
     * * @return A shared_ptr to the resolved IMaterial, or nullptr if the key is
     * missing or the identifier does not exist in the material map.
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
