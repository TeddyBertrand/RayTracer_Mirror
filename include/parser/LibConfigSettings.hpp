#pragma once
#include "math/Vector3D.hpp"
#include "parser/ISettings.hpp"
#include <libconfig.h++>
#include <memory>
#include <string>

namespace Raytracer {
/**
 * @brief Implementation Header-Only of ISetting for libconfig++
 */
class LibconfigSetting : public ISetting {
public:
    explicit LibconfigSetting(const libconfig::Setting& setting) : _setting(setting) {}

    virtual ~LibconfigSetting() = default;

    bool exists(const std::string& path) const override { return _setting.exists(path); }

    int getInt(const std::string& path) const override {
        return static_cast<int>(_setting.lookup(path));
    }
    int getInt(const std::string& path, int defaultValue) const override {
        return exists(path) ? getInt(path) : defaultValue;
    }

    double getFloat(const std::string& path) const override {
        return static_cast<double>(_setting.lookup(path));
    }
    double getFloat(const std::string& path, double defaultValue) const override {
        return exists(path) ? getFloat(path) : defaultValue;
    }

    std::string getString(const std::string& path) const override {
        return std::string(static_cast<const char*>(_setting.lookup(path)));
    }
    std::string getString(const std::string& path, const std::string& defaultValue) const override {
        return exists(path) ? getString(path) : defaultValue;
    }
    std::string getString(const std::string& path, const char* defaultValue) const override {
        return exists(path) ? getString(path) : std::string(defaultValue);
    }

    bool getBool(const std::string& path) const override {
        return static_cast<bool>(_setting.lookup(path));
    }
    bool getBool(const std::string& path, bool defaultValue) const override {
        return exists(path) ? getBool(path) : defaultValue;
    }

    Vector3D getVector(const std::string& path) const override {
        const libconfig::Setting& v = _setting.lookup(path);
        return Vector3D(
            static_cast<double>(v["x"]), static_cast<double>(v["y"]), static_cast<double>(v["z"]));
    }
    Vector3D getVector(const std::string& path, const Vector3D& defaultValue) const override {
        return exists(path) ? getVector(path) : defaultValue;
    }

    Color getColor(const std::string& path) const override {
        const libconfig::Setting& v = _setting.lookup(path);
        return Color(
            static_cast<double>(v["r"]), static_cast<double>(v["g"]), static_cast<double>(v["b"]));
    }
    Color getColor(const std::string& path, const Color& defaultValue) const override {
        return exists(path) ? getColor(path) : defaultValue;
    }

    std::shared_ptr<ISetting> getGroup(const std::string& path) const override {
        return std::make_shared<LibconfigSetting>(_setting.lookup(path));
    }

private:
    const libconfig::Setting& _setting;
};
} // namespace Raytracer
