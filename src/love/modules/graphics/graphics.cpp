#include <sol/sol.hpp>
#include <grrlib.h>
#include <ogc/conf.h>
#include <tuple>
#include <string>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <cstdint>

#include "classes/texture.hpp"
#include "classes/font.hpp"
#include "classes/quad.hpp"

#include "graphics.hpp"
#include "Vera_ttf.h"

namespace {
    unsigned int color             = 0xffffffff;
    unsigned int backgroundColor   = 0xff000000;
    love::graphics::Font *curFont  = nullptr;
    
    int width  = 640;
    int height = 480;
}

namespace love {
    namespace graphics {
        void __init(sol::state &luastate) {
            GRRLIB_Init();

            // set default font
            curFont = new love::graphics::Font();

            __registerTypes(luastate);
        }

        void __registerTypes(sol::state &luastate) {
            luastate.new_usertype<love::graphics::Texture>(
                "Texture",
                sol::no_constructor,
                "getWidth", &love::graphics::Texture::getWidth,
                "getHeight", &love::graphics::Texture::getHeight,
                "getDimensions", &love::graphics::Texture::getDimensions
            );
            
            luastate.new_usertype<love::graphics::Font>(
                "Font",
                sol::no_constructor,
                "getWidth", &love::graphics::Font::getWidth,
                "getHeight", &love::graphics::Font::getHeight
            );

            luastate.new_usertype<love::graphics::Quad>(
                "Quad",
                sol::no_constructor,
                "getViewport", &love::graphics::Quad::getViewport,
                "setViewport", &love::graphics::Quad::setViewport
            );
        }

        static GXColor make_gxcolor_from_uint32(unsigned int packed) {
            GXColor c;
            c.r = static_cast<uint8_t>((packed >> 24) & 0xFF);
            c.g = static_cast<uint8_t>((packed >> 16) & 0xFF);
            c.b = static_cast<uint8_t>((packed >> 8) & 0xFF);
            c.a = static_cast<uint8_t>(packed & 0xFF);
            return c;
        }

        static int16_t float_to_int16_rounded(float v) {
            return static_cast<int16_t>(std::lround(v));
        }

        #pragma region Color

        void _setColor(float r, float g, float b, float a) {
            unsigned int r_int = static_cast<unsigned int>(r * 255);
            unsigned int g_int = static_cast<unsigned int>(g * 255);
            unsigned int b_int = static_cast<unsigned int>(b * 255);
            unsigned int a_int = static_cast<unsigned int>(a * 255);
            color = (r_int << 24) | (g_int << 16) | (b_int << 8) | a_int;
        }

        void setColor_float4(float r, float g, float b, float a) {
            _setColor(r, g, b, a);
        }

        void setColor_float3(float r, float g, float b) {
            _setColor(r, g, b, 1.0f);
        }

        void _setBackgroundColor(float r, float g, float b, float a) {
            unsigned int r_int = static_cast<unsigned int>(r * 255);
            unsigned int g_int = static_cast<unsigned int>(g * 255);
            unsigned int b_int = static_cast<unsigned int>(b * 255);
            unsigned int a_int = static_cast<unsigned int>(a * 255);
            backgroundColor = (r_int << 24) | (g_int << 16) | (b_int << 8) | a_int;
        }

        void setBackgroundColor_float4(float r, float g, float b, float a) {
            _setBackgroundColor(r, g, b, a);
        }

        void setBackgroundColor_float3(float r, float g, float b) {
            _setBackgroundColor(r, g, b, 1.0f);
        }

        std::tuple<float, float, float, float> getBackgroundColor() {
            float r = (backgroundColor >> 24) & 0xFF;
            float g = (backgroundColor >> 16) & 0xFF;
            float b = (backgroundColor >> 8) & 0xFF;
            float a = backgroundColor & 0xFF;
            return std::make_tuple(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
        }

        std::tuple<float, float, float, float> getColor() {
            float r = (color >> 24) & 0xFF;
            float g = (color >> 16) & 0xFF;
            float b = (color >> 8) & 0xFF;
            float a = color & 0xFF;
            return std::make_tuple(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
        }

        #pragma region Shapes

        void rectangle(const std::string &mode, float x, float y, float width, float height) {
            GRRLIB_Rectangle(x, y, width, height, color, mode.compare("fill") == 0);
        }

        #pragma region Textures

        void _draw(love::graphics::Texture &texture,
            float x, float y, float rotation, float sx, float sy,
            float ox, float oy
        ) {
            
            if (ox != 0 || oy != 0) {
                x -= ox * sx;
                y -= oy * sy;
                float cos_r = cos(rotation);
                float sin_r = sin(rotation);
                x += ox * cos_r - oy * sin_r;
                y += ox * sin_r + oy * cos_r;
            }
            GRRLIB_DrawImg(x, y, texture.texture, rotation, sx, sy, color);
        }

        void _draw_quad(love::graphics::Texture &texture, love::graphics::Quad &quad,
            float x, float y, float rotation, float sx, float sy,
            float ox, float oy
        ) {
            if (ox != 0 || oy != 0) {
                x -= ox * sx;
                y -= oy * sy;
                float cos_r = cos(rotation);
                float sin_r = sin(rotation);
                x += ox * cos_r - oy * sin_r;
                y += ox * sin_r + oy * cos_r;
            }

            GRRLIB_DrawPart(
                x, y,
                quad.quadX, quad.quadY, quad.quadWidth, quad.quadHeight,
                texture.texture, rotation, sx, sy, color
            );
        }

        void draw(love::graphics::Texture &texture) {
            _draw(texture, 0, 0, 0, 1, 1, 0, 0);
        }
        void draw_x(love::graphics::Texture &texture, float x) {
            _draw(texture, x, 0, 0, 1, 1, 0, 0);
        }
        void draw_x_y(love::graphics::Texture &texture, float x, float y) {
            _draw(texture, x, y, 0, 1, 1, 0, 0);
        }
        void draw_x_y_r(love::graphics::Texture &texture, float x, float y, float rotation) {
            _draw(texture, x, y, rotation, 1, 1, 0, 0);
        }
        void draw_x_y_r_sx(love::graphics::Texture &texture, float x, float y, float rotation, float sx) {
            _draw(texture, x, y, rotation, sx, 1, 0, 0);
        }
        void draw_x_y_r_sx_sy(love::graphics::Texture &texture, float x, float y, float rotation, float sx, float sy) {
            _draw(texture, x, y, rotation, sx, sy, 0, 0);
        }
        void draw_x_y_r_sx_sy_ox(love::graphics::Texture &texture, float x, float y, float rotation, float sx, float sy, float ox) {
            _draw(texture, x, y, rotation, sx, sy, ox, 0);
        }
        void draw_x_y_r_sx_sy_ox_oy(love::graphics::Texture &texture, float x, float y, float rotation, float sx, float sy, float ox, float oy) {
            _draw(texture, x, y, rotation, sx, sy, ox, oy);
        }

        void draw_quad(love::graphics::Texture &texture, love::graphics::Quad &quad) {
            _draw_quad(texture, quad, 0, 0, 0, 1, 1, 0, 0);
        }
        void draw_quad_x(love::graphics::Texture &texture, love::graphics::Quad &quad, float x) {
            _draw_quad(texture, quad, x, 0, 0, 1, 1, 0, 0);
        }

        void draw_quad_x_y(love::graphics::Texture &texture, love::graphics::Quad &quad, float x, float y) {
            _draw_quad(texture, quad, x, y, 0, 1, 1, 0, 0);
        }

        void draw_quad_x_y_r(love::graphics::Texture &texture, love::graphics::Quad &quad, float x, float y, float rotation) {
            _draw_quad(texture, quad, x, y, rotation, 1, 1, 0, 0);
        }

        void draw_quad_x_y_r_sx(love::graphics::Texture &texture, love::graphics::Quad &quad, float x, float y, float rotation, float sx) {
            _draw_quad(texture, quad, x, y, rotation, sx, 1, 0, 0);
        }

        void draw_quad_x_y_r_sx_sy(love::graphics::Texture &texture, love::graphics::Quad &quad, float x, float y, float rotation, float sx, float sy) {
            _draw_quad(texture, quad, x, y, rotation, sx, sy, 0, 0);
        }

        void draw_quad_x_y_r_sx_sy_ox(love::graphics::Texture &texture, love::graphics::Quad &quad, float x, float y, float rotation, float sx, float sy, float ox) {
            _draw_quad(texture, quad, x, y, rotation, sx, sy, ox, 0);
        }

        void draw_quad_x_y_r_sx_sy_ox_oy(love::graphics::Texture &texture, love::graphics::Quad &quad, float x, float y, float rotation, float sx, float sy, float ox, float oy) {
            _draw_quad(texture, quad, x, y, rotation, sx, sy, ox, oy);
        }

        love::graphics::Texture newImage(std::string file) {
            return love::graphics::Texture(file);
        }

        love::graphics::Texture newImage_data(const std::vector<uint8_t>& data) {
            return love::graphics::Texture(data);
        }

        love::graphics::Quad newQuad(float x, float y, float width, float height, float sw, float sh) {
            return love::graphics::Quad(x, y, width, height, sw, sh);
        }

        #pragma region Fonts
        void setFont(love::graphics::Font &font) {
            curFont = &font;
        }

        love::graphics::Font newFont() {
            return love::graphics::Font();
        }
        love::graphics::Font newFont_size(int size) {
            return love::graphics::Font(size);
        }
        love::graphics::Font newFont_file(std::string file) {
            return love::graphics::Font(file);
        }
        love::graphics::Font newFont_file_size(std::string file, int size) {
            return love::graphics::Font(file, size);
        }

        void _print(const std::string &text, float x, float y, float rotation, float sx, float sy, float ox, float oy) {
            if (!curFont) {
                throw std::runtime_error("No font set for printing text.");
            }
            if (ox != 0 || oy != 0) {
                x -= ox * sx;
                y -= oy * sy;
                float cos_r = cos(rotation);
                float sin_r = sin(rotation);
                x += ox * cos_r - oy * sin_r;
                y += ox * sin_r + oy * cos_r;
            }
            GXColor gxColor = make_gxcolor_from_uint32(color);
            auto wide = utf8_to_wchar_vec(text);
            curFont->font->drawText(float_to_int16_rounded(x), float_to_int16_rounded(y), wide.data(), gxColor, FTGX_NULL);
        }

        void print(const std::string &text) {
            _print(text, 0, 0, 0, 1, 1, 0, 0);
        }

        void print_x(const std::string &text, float x) {
            _print(text, x, 0, 0, 1, 1, 0, 0);
        }

        void print_x_y(const std::string &text, float x, float y) {
            _print(text, x, y, 0, 1, 1, 0, 0);
        }

        void print_x_y_r(const std::string &text, float x, float y, float rotation) {
            _print(text, x, y, rotation, 1, 1, 0, 0);
        }

        void print_x_y_r_sx(const std::string &text, float x, float y, float rotation, float sx) {
            _print(text, x, y, rotation, sx, 1, 0, 0);
        }

        void print_x_y_r_sx_sy(const std::string &text, float x, float y, float rotation, float sx, float sy) {
            _print(text, x, y, rotation, sx, sy, 0, 0);
        }

        void print_x_y_r_sx_sy_ox(const std::string &text, float x, float y, float rotation, float sx, float sy, float ox) {
            _print(text, x, y, rotation, sx, sy, ox, 0);
        }

        void print_x_y_r_sx_sy_ox_oy(const std::string &text, float x, float y, float rotation, float sx, float sy, float ox, float oy) {
            _print(text, x, y, rotation, sx, sy, ox, oy);
        }

        #pragma region Helpers

        bool isActive() {
            return true;
        }

        void origin() {
            // reset color
            color = 0xffffffff;
        }

        void _clear(float r, float g, float b, float a) {
            unsigned int r_int = static_cast<unsigned int>(r * 255);
            unsigned int g_int = static_cast<unsigned int>(g * 255);
            unsigned int b_int = static_cast<unsigned int>(b * 255);
            unsigned int a_int = static_cast<unsigned int>(a * 255);

            GRRLIB_FillScreen((r_int << 24) | (g_int << 16) | (b_int << 8) | a_int);
        }

        void clear_float4(float r, float g, float b, float a) {
            _clear(r, g, b, a);
        }

        void clear_float3(float r, float g, float b) {
            _clear(r, g, b, 1.0f);
        }

        void clear() {
            _clear(0, 0, 0, 1.0f);
        }

        void present() {
            GRRLIB_Render();
        }

        int getWidth() {
            return width;
        }

        int getHeight() {
            return height;
        }

        std::tuple<int, int> getDimensions() {
            return std::make_tuple(getWidth(), getHeight());
        }

        #pragma endregion
    }
}
