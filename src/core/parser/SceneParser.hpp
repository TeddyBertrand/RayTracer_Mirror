#pragma once
#include "components/SceneManager.hpp"
#include "core/scene/Scene.hpp"
#include "factory/SceneFactories.hpp"
#include "math/Color.hpp"
#include "math/Matrix.hpp"
#include "math/Vector3D.hpp"
#include "parser/LibConfigSettings.hpp"
#include "parser/PrimitiveSettings.hpp"
#include <iostream>
#include <libconfig.h++>
#include <unordered_map>
#include <vector>

namespace Raytracer {

class SceneParser {
public:
    explicit SceneParser(SceneFactories& factories) : _factories(factories) {}

    int getRenderSamples() const { return _renderSamples; }
    double getRenderThreshold() const { return _renderThreshold; }
    int getAOSamples() const { return _aoSamples; }
    double getAOMaxDistance() const { return _aoMaxDistance; }

    void loadScene(const std::string& filePath, Scene& outScene);

    class SceneParserException : public std::exception {
    public:
        explicit SceneParserException(const std::string& msg) : _msg(msg) {}
        [[nodiscard]] const char* what() const noexcept override { return _msg.c_str(); }

    private:
        std::string _msg;
    };

    class RenderSettingsException : public SceneParserException {
    public:
        explicit RenderSettingsException(const std::string& msg) : SceneParserException(msg) {}
    };

private:
    // Signature unifiée pour le dispatch table
    void parseCamera(const libconfig::Setting& setting, Scene& outScene);
    void parseShapes(const libconfig::Setting& setting, Scene& outScene);
    void parseMaterials(const libconfig::Setting& setting, Scene& outScene);
    void parseLights(const libconfig::Setting& setting, Scene& outScene);
    void parseSky(const libconfig::Setting& setting, Scene& outScene);
    void parseRender(const libconfig::Setting& setting, Scene& outScene);

    std::shared_ptr<IPrimitive> handleImport(const libconfig::Setting& setting, Scene& outScene);
    std::shared_ptr<IPrimitive> handleStandardPrimitive(const libconfig::Setting& setting,
                                                        Scene& outScene);

    Matrix parseMatrix(const libconfig::Setting& setting);

    SceneFactories& _factories;
    SceneManager _manager;

    int _renderSamples = 16;
    double _renderThreshold = 0.1;
    int _aoSamples = 0;
    double _aoMaxDistance = 10.0;

    using SectionParser = void (SceneParser::*)(const libconfig::Setting&, Scene&);
    using SectionTable = std::unordered_map<std::string, SectionParser>;

    static inline const SectionTable _sectionDispatch = {{"sky", &SceneParser::parseSky},
                                                         {"camera", &SceneParser::parseCamera},
                                                         {"render", &SceneParser::parseRender},
                                                         {"shapes", &SceneParser::parseShapes},
                                                         {"lights", &SceneParser::parseLights}};
};

} // namespace Raytracer
