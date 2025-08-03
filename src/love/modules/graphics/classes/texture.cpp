#include "../../lib/sol/sol.hpp"
#include <grrlib.h>

#include "../../../modules/filesystem/filesystem.hpp"

#include "texture.hpp"

namespace love {
    namespace graphics {
        Texture::Texture(std::string file) {
            std::string path = filesystem::getFilePath(file);
            if (!love::filesystem::fileExists(path)) {
                throw std::runtime_error("Texture file not found: " + path);
            }

            texture = GRRLIB_LoadTextureFromFile(path.c_str());
            if (!texture) {
                throw std::runtime_error("Failed to load texture: " + path);
            }
            width = texture->w;
            height = texture->h;
        }

        int Texture::getWidth() const {
            return width;
        }

        int Texture::getHeight() const {
            return height;
        }

        std::tuple<int, int> Texture::getDimensions() const {
            return std::make_tuple(width, height);
        }
    }
}
