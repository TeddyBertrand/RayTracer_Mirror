#include "Image.hpp"

namespace Raytracer
{

void Image::drawFromBuffer(const FrameBuffer& buffer, const std::string& filename)
{
    std::ofstream file(filename);
    file << "P3\n" << _width << " " << _height << "\n255\n";

    for (const auto& color : buffer) {
        int ir = Color::to_byte(color.r);
        int ig = Color::to_byte(color.g);
        int ib = Color::to_byte(color.b);

        file << ir << " " << ig << " " << ib << "\n";
    }
}

} // namespace Raytracer
