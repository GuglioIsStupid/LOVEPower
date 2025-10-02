#pragma once

#include <FreeTypeGX/FreeTypeGX.h>
#include <string>
#include <vector>

#include "../../../common/Object.h"

namespace {
    [[maybe_unused]] 
    std::vector<wchar_t> utf8_to_wchar_vec(std::string s) {
        size_t len = mbstowcs(nullptr, s.c_str(), 0);
        if (len == static_cast<size_t>(-1)) {
            std::vector<wchar_t> fallback(s.size() + 1);
            for (size_t i = 0; i < s.size(); ++i) fallback[i] = static_cast<unsigned char>(s[i]);
            fallback[s.size()] = 0;
            return fallback;
        }
        std::vector<wchar_t> buf(len + 1);
        mbstowcs(buf.data(), s.c_str(), len + 1);
        buf[len] = 0;
        return buf;
    }
}

namespace love {
    namespace graphics {
        struct Font : public Object {
            void       _createFont(const uint8_t* newFont, int dataSize, int size);

            FreeTypeGX *font;

            Font();
            Font(int size);
            Font(std::string file);
            Font(std::string file, int size);

            int getWidth(std::string text);
            int getHeight();
        };
    }
}
