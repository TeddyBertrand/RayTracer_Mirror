#include "EmptySky.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {
extern "C" {

const char* getName() { return "empty"; }

ISky* createPlugin([[maybe_unused]] const ISetting& settings) { return new EmptySky(); }
}
} // namespace Raytracer
