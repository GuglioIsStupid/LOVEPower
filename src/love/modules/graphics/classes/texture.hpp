#pragma once

#include <tuple>
#include <string>

#include <sol/sol.hpp>
#include <grrlib.h>
#include "../../data/data.hpp"
#include "../../../common/Object.h"

namespace love {
    namespace graphics {
        struct Texture {
            GRRLIB_texImg *texture;

            int width;
            int height;

            std::string filterMin;
            std::string filterMag;

            int                  getWidth()      const;
            int                  getHeight()     const;
            std::tuple<int, int> getDimensions() const;

            void setFilter_string1(std::string min);
            void setFilter_string2(std::string min, std::string mag);

            Texture(std::string file);
            Texture(love::data::Data data);
            Texture();
        };
    }
}