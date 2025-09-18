#pragma once

#include <tuple>

namespace love {
    namespace graphics {
        struct Quad {
            float quadX;
            float quadY;
            float quadWidth;
            float quadHeight;
            float textureWidth;
            float textureHeight;

            Quad(float x, float y, float width, float height, float sw, float sh);

            void setViewport(float x, float y, float width, float height, float sw, float sh);
            std::tuple<float, float, float, float> getViewport();

            std::tuple<float, float> getTextureDimensions();
        };
    }
}