#pragma once

#include <tuple>
#include <string>

#include <sol/sol.hpp>
#include <grrlib.h>

namespace love {
    namespace graphics {
        struct Texture {
            GRRLIB_texImg *texture;

            int width;
            int height;


            int                  getWidth()      const;
            int                  getHeight()     const;
            std::tuple<int, int> getDimensions() const;

            Texture(std::string file);
            Texture(const std::vector<uint8_t>& data);
        };
    }
}