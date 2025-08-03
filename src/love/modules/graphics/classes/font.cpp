#include "../../lib/sol/sol.hpp"
#include "../../lib/FreeTypeGX/FreeTypeGX.h"
#include <grrlib.h>

#include "../../../modules/filesystem.hpp"

#include "font.hpp"

#include "Vera_ttf.h"

#define DEFAULT_FONT_SIZE 12

namespace love {
    namespace graphics {

        void Font::_createFont(const uint8_t* newFont, int dataSize, int size) {
            font = new FreeTypeGX();
            font->loadFont(newFont, dataSize, size);
        }

        Font::Font() {
            _createFont(Vera_ttf, static_cast<int>(Vera_ttf_size), DEFAULT_FONT_SIZE);
        }

        Font::Font(int size) {
            _createFont(Vera_ttf, static_cast<int>(Vera_ttf_size), size);
        }

        Font::Font(std::string file) {
            uint8_t* data = nullptr;
            int dataSize = GRRLIB_LoadFile(filesystem::getFilePath(file).c_str(), &data);
            if (dataSize <= 0) {
                throw std::runtime_error("Font file not found: " + filesystem::getFilePath(file));
            }
            _createFont(data, dataSize, DEFAULT_FONT_SIZE);
        }

        Font::Font(std::string file, int size) {
            uint8_t* data = nullptr;
            int dataSize = GRRLIB_LoadFile(filesystem::getFilePath(file).c_str(), &data);
            if (dataSize <= 0) {
                throw std::runtime_error("Font file not found: " + filesystem::getFilePath(file));
            }
            _createFont(data, dataSize, size);
        }

        int Font::getWidth(std::string text) {
            auto wide = utf8_to_wchar_vec(text);
            return font->getWidth(wide.data());
        }

        int Font::getHeight() {
            auto wide = utf8_to_wchar_vec("");
            return font->getHeight(wide.data());
        }
    }
}
