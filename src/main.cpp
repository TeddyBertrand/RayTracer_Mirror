#include "core/Raytracer.hpp"

int main(int argc, const char *argv[])
{
    Raytracer::Raytracer raytracer(argc, argv);

    raytracer.run();
    return raytracer.getStatus();
}
