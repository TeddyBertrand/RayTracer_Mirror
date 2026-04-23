#pragma once
#include "render/FrameBuffer.hpp"
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

namespace Raytracer {

class Image {
public:
    Image(int width, int height) : _width(width), _height(height) {}

    void drawFromBuffer(const FrameBuffer& buffer, const std::string& filename = "output.ppm");

private:
    int _width;
    int _height;
};

} // namespace Raytracer
