#pragma once

#include "factory/IFactory.hpp"
#include <unordered_map>

namespace Raytracer
{
    template<typename T>
    class AFactory : public virtual IFactory<T>
    {
        public:
            virtual ~AFactory() = default;

            virtual std::shared_ptr<T> create(const std::string &name, const ISetting& settings) override = 0;

            void registerType(const std::string &name, std::function<std::shared_ptr<T>(const ISetting&)> init) {
                _factoryMap[name] = init;
            }
            
        protected:
            std::unordered_map<std::string, std::function<std::shared_ptr<T>(const ISetting& settings)>> _factoryMap;
    };
};
