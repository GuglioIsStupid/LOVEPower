#include "quad.hpp"
#include <tuple>

namespace love {
    namespace graphics {
        Quad::Quad(float x, float y, float width, float height, float sw, float sh) {
            setViewport(x, y, width, height, sw, sh);
        }

        void Quad::setViewport(float x, float y, float width, float height, float sw, float sh) {
            quadX = x;
            quadY = y;
            quadWidth = width;
            quadHeight = height;
            textureWidth = sw;
            textureHeight = sh;
        }

        std::tuple<float, float, float, float> Quad::getViewport() {
            return std::make_tuple(quadX, quadY, quadWidth, quadHeight);
        }

        std::tuple<float, float> Quad::getTextureDimensions() {
            return std::make_tuple(textureWidth, textureHeight);
        }
    }
}