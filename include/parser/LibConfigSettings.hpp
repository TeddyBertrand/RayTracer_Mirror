#pragma once
#include "math/Vector3D.hpp"
#include "parser/ISettings.hpp"
#include <libconfig.h++>
#include <memory>
#include <string>

namespace Raytracer {
/**
 * @brief Implémentation Header-Only de ISetting pour libconfig++
 */
class LibconfigSetting : public ISetting {
public:
    explicit LibconfigSetting(const libconfig::Setting& setting) : _setting(setting) {}

    virtual ~LibconfigSetting() = default;

    int getInt(const std::string& path) const override {
        return static_cast<int>(_setting.lookup(path));
    }

    double getFloat(const std::string& path) const override {
        return static_cast<double>(_setting.lookup(path));
    }

    std::string getString(const std::string& path) const override {
        return std::string(static_cast<const char*>(_setting.lookup(path)));
    }

    bool getBool(const std::string& path) const override {
        return static_cast<bool>(_setting.lookup(path));
    }

    Vector3D getVector(const std::string& path) const override {
        const libconfig::Setting& v = _setting.lookup(path);
        return Vector3D(
            static_cast<double>(v["x"]), static_cast<double>(v["y"]), static_cast<double>(v["z"]));
    }

    Color getColor(const std::string& path) const override {
        const libconfig::Setting& v = _setting.lookup(path);
        return Color(
            static_cast<double>(v["r"]), static_cast<double>(v["g"]), static_cast<double>(v["b"]));
    }

    bool exists(const std::string& path) const override { return _setting.exists(path); }

    std::shared_ptr<ISetting> getGroup(const std::string& path) const override {
        return std::make_shared<LibconfigSetting>(_setting.lookup(path));
    }

private:
    const libconfig::Setting& _setting;
};
} // namespace Raytracer
