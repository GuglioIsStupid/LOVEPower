#pragma once

#include <sol/sol.hpp>

#include <tuple>
#include <string>

#include "classes/texture.hpp"
#include "classes/font.hpp"
#include "classes/quad.hpp"

extern "C" {
    #include <lua.h>
}

#include "../data/data.hpp"

namespace love {
    namespace graphics {
        void __init(sol::state &luastate);
        void __registerTypes(sol::state &luastate);

        #pragma region Color
        void _setColor(float r, float g, float b, float a);
        void setColor_float4(float r, float g, float b, float a);
        void setColor_float3(float r, float g, float b);
        void _setBackgroundColor(float r, float g, float b, float a);
        void setBackgroundColor_float4(float r, float g, float b, float a);
        void setBackgroundColor_float3(float r, float g, float b);

        std::tuple<float, float, float, float> getColor();
        std::tuple<float, float, float, float> getBackgroundColor();

        #pragma region Shapes

        void rectangle(const std::string &mode, float x, float y, float width, float height);

        #pragma region Textures

        void _draw(love::graphics::Texture &texture, float x, float y, float rotation, float sx, float sy, float ox, float oy);
        void _draw_quad(love::graphics::Texture &texture, love::graphics::Quad &quad, float x, float y, float rotation, float sx, float sy, float ox, float oy);

        void draw(love::graphics::Texture &texture);
        void draw_x(love::graphics::Texture &texture, float x);
        void draw_x_y(love::graphics::Texture &texture, float x, float y);
        void draw_x_y_r(love::graphics::Texture &texture, float x, float y, float rotation);
        void draw_x_y_r_sx(love::graphics::Texture &texture, float x, float y, float rotation, float sx);
        void draw_x_y_r_sx_sy(love::graphics::Texture &texture, float x, float y, float rotation, float sx, float sy);
        void draw_x_y_r_sx_sy_ox(love::graphics::Texture &texture, float x, float y, float rotation, float sx, float sy, float ox);
        void draw_x_y_r_sx_sy_ox_oy(love::graphics::Texture &texture, float x, float y, float rotation, float sx, float sy, float ox, float oy);

        void draw_quad(love::graphics::Texture &texture, love::graphics::Quad &quad);
        void draw_quad_x(love::graphics::Texture &texture, love::graphics::Quad &quad, float x);
        void draw_quad_x_y(love::graphics::Texture &texture, love::graphics::Quad &quad, float x, float y);
        void draw_quad_x_y_r(love::graphics::Texture &texture, love::graphics::Quad &quad, float x, float y, float rotation);
        void draw_quad_x_y_r_sx(love::graphics::Texture &texture, love::graphics::Quad &quad, float x, float y, float rotation, float sx);
        void draw_quad_x_y_r_sx_sy(love::graphics::Texture &texture, love::graphics::Quad &quad, float x, float y, float rotation, float sx, float sy);
        void draw_quad_x_y_r_sx_sy_ox(love::graphics::Texture &texture, love::graphics::Quad &quad, float x, float y, float rotation, float sx, float sy, float ox);
        void draw_quad_x_y_r_sx_sy_ox_oy(love::graphics::Texture &texture, love::graphics::Quad &quad, float x, float y, float rotation, float sx, float sy, float ox, float oy);

        love::graphics::Texture newImage(std::string file);
        love::graphics::Texture newImage_data(love::data::Data data);

        love::graphics::Quad newQuad(float x, float y, float width, float height, float sw, float sh);

        #pragma region Fonts
        
        void setFont(love::graphics::Font &font);
        love::graphics::Font newFont();
        love::graphics::Font newFont_size(int size);
        love::graphics::Font newFont_file(std::string file);
        love::graphics::Font newFont_file_size(std::string file, int size);

        void _print(const std::string &text, float x, float y, float rotation, float sx, float sy, float ox, float oy);
        void print(const std::string &text);
        void print_x(const std::string &text, float x);
        void print_x_y(const std::string &text, float x, float y);
        void print_x_y_r(const std::string &text, float x, float y, float rotation);
        void print_x_y_r_sx(const std::string &text, float x, float y, float rotation, float sx);
        void print_x_y_r_sx_sy(const std::string &text, float x, float y, float rotation, float sx, float sy);
        void print_x_y_r_sx_sy_ox(const std::string &text, float x, float y, float rotation, float sx, float sy, float ox);
        void print_x_y_r_sx_sy_ox_oy(const std::string &text, float x, float y, float rotation, float sx, float sy, float ox, float oy);

        #pragma region Helpers

        bool isActive();

        void origin();
        void present();

        void _clear(float r, float g, float b, float a);
        void clear_float4(float r, float g, float b, float a);
        void clear_float3(float r, float g, float b);
        void clear();

        void push();
        void pop();
        void translate(float x, float y);
        void rotate(float r);
        void scale(float sx, float sy);

        int getWidth();
        int getHeight();
        std::tuple<int, int> getDimensions();

        #pragma endregion
    }
}

int luaopen_love_graphics(lua_State *L);