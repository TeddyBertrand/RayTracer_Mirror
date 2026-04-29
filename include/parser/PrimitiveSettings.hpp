#pragma once

#include "components/IMaterial.hpp"
#include "parser/ISettings.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace Raytracer {

class PrimitiveSetting : public ISetting {
public:
    PrimitiveSetting(const ISetting& base,
                     const std::unordered_map<std::string, std::shared_ptr<IMaterial>>& materials)
        : _base(base), _materials(materials) {}

    double getFloat(const std::string& path) const override { return _base.getFloat(path); }
    int getInt(const std::string& path) const override { return _base.getInt(path); }
    Vector3D getVector(const std::string& path) const override { return _base.getVector(path); }
    std::string getString(const std::string& path) const override { return _base.getString(path); }

    bool getBool(const std::string& path) const override { return _base.getBool(path); }
    Color getColor(const std::string& path) const override { return _base.getColor(path); }
    bool exists(const std::string& path) const override { return _base.exists(path); }
    std::shared_ptr<ISetting> getGroup(const std::string& path) const override {
        return _base.getGroup(path);
    }

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
