#include <sol/sol.hpp>
#include <grrlib.h>

#include "../../filesystem/filesystem.hpp"
#include "../../data/data.hpp"
#include "texture.hpp"

namespace love {
    namespace graphics {
        Texture::Texture(std::string file) {
            std::string path = filesystem::getFilePath(file);

            texture = GRRLIB_LoadTextureFromFile(path.c_str());
            if (!texture) {
                throw std::runtime_error("Failed to load texture: " + path);
            }
            width = texture->w;
            height = texture->h;
        }

        Texture::Texture(love::data::Data data) {
           if (data.empty()) {
                throw std::runtime_error("Cannot create texture from empty data");
            }

            std::vector<uint8_t> tempBuffer(data.begin(), data.end());

            texture = GRRLIB_LoadTexture(tempBuffer.data());
            if (!texture) {
                throw std::runtime_error("Failed to load texture from memory data");
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
