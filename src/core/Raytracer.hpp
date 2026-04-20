#pragma once

#include "math/Ray.hpp"
#include "core/renderer/Renderer.hpp"

namespace Raytracer
{

class Raytracer
{
public:
    Raytracer(int argc, const char **argv);
    ~Raytracer() = default;

    void run();
    int getStatus() const { return _exitCode; }

private:
    static constexpr int SUCCESS_STATUS = 0;
    static constexpr int ERROR_STATUS = 84;

private:
    int _exitCode = SUCCESS_STATUS;

private:
    Renderer _renderer;
};

} // namespace Raytracer
