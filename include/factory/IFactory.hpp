#pragma once

#include "components/IPlugin.hpp"
#include "parser/ISettings.hpp"
#include <functional>
#include <memory>
#include <string>

namespace Raytracer {
template <typename T> class IFactory {
public:
    virtual std::shared_ptr<T> create(const std::string& name, const ISetting& settings) = 0;
};
}; // namespace Raytracer
