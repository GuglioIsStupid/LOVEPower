#pragma once

#include <tuple>

namespace love {
    namespace graphics {
        struct Transform {
            float x, y, sx, sy, r;

            Transform();

            Transform clone() const;

            void translate(float dx, float dy);
            void rotate(float dr);
            void scale(float dsx, float dsy);
        };
    }
}