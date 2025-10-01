#include "transform.hpp"

namespace love {
    namespace graphics {
        Transform::Transform() {
            x = 0;
            y = 0;
            sx = 1;
            sy = 1;
            r = 0;
        }

        Transform Transform::clone() const {
            Transform t;
            t.x = x;
            t.y = y;
            t.sx = sx;
            t.sy = sy;
            t.r = r;
            return t;
        }

        void Transform::translate(float dx, float dy) {
            x += dx;
            y += dy;
        }

        void Transform::rotate(float dr) {
            r += dr;
        }

        void Transform::scale(float dsx, float dsy) {
            sx *= dsx;
            sy *= dsy;
        }
    }
}