#pragma once

#include "components/IPlugin.hpp"
#include "parser/ISettings.hpp"
#include <memory>
#include <string>
#include <functional>

namespace Raytracer
{
    template<typename T>
    class IFactory
    {
        public:
            virtual std::shared_ptr<T> create(const std::string &name, const ISetting& settings) = 0;
    };
};
