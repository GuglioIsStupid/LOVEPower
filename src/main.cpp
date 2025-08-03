#define SOL_ALL_SAFETIES_ON 1

#include "lib/sol/sol.hpp"
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <string>

#include "love/modules/graphics/classes/texture.hpp"
#include "love/modules/graphics/classes/font.hpp"

#include "love/love.hpp"
#include "love/modules/graphics/graphics.hpp"
#include "love/modules/filesystem.hpp"
#include "love/modules/timer.hpp"
#include "love/modules/system.hpp"
#include "love/modules/audio.hpp"

#include "boot_lua.h"

int main(int argc, char** argv) {
    sol::state luastate;
    luastate.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::coroutine,
        sol::lib::string,
        sol::lib::os,
        sol::lib::math,
        sol::lib::table,
        sol::lib::debug
    );

    love::graphics::__init(luastate);
    love::filesystem::__init(luastate, argc, argv);
    love::timer::__init(luastate);

    luastate["love"] = luastate.create_table_with(
        "_console", love::_console,
        "_os", love::_os,
        "hasDeprecationOutput", love::hasDeprecationOutput,
        "setDeprecationOutput", love::setDeprecationOutput,
        "getVersion", love::getVersion,
        "graphics", luastate.create_table_with(
            "setColor", sol::overload(
                love::graphics::setColor_float4,
                love::graphics::setColor_float3
            ),
            "getColor", love::graphics::getColor,
            "rectangle", love::graphics::rectangle,
            "draw", sol::overload( // TODO: Maybe use sol::variadic_args for multiple argument functions?
                love::graphics::draw,
                love::graphics::draw_x,
                love::graphics::draw_x_y,
                love::graphics::draw_x_y_r,
                love::graphics::draw_x_y_r_sx,
                love::graphics::draw_x_y_r_sx_sy,
                love::graphics::draw_x_y_r_sx_sy_ox,
                love::graphics::draw_x_y_r_sx_sy_ox_oy
            ),
            "setFont", love::graphics::setFont,
            "newFont", sol::overload(
                love::graphics::newFont,
                love::graphics::newFont_size,
                love::graphics::newFont_file,
                love::graphics::newFont_file_size
            ),
            "print", sol::overload(
                love::graphics::print,
                love::graphics::print_x,
                love::graphics::print_x_y,
                love::graphics::print_x_y_r,
                love::graphics::print_x_y_r_sx,
                love::graphics::print_x_y_r_sx_sy,
                love::graphics::print_x_y_r_sx_sy_ox,
                love::graphics::print_x_y_r_sx_sy_ox_oy
            ),
            "newImage", love::graphics::newImage,
            "setBackgroundColor", sol::overload(
                love::graphics::setBackgroundColor_float4,
                love::graphics::setBackgroundColor_float3
            ),
            "getBackgroundColor", love::graphics::getBackgroundColor,
            /* "translate", love::graphics::translate,
            "rotate", love::graphics::rotate,
            "scale", love::graphics::scale,
            "push", love::graphics::push,
            "pop", love::graphics::pop, */
            "clear", sol::overload(
                love::graphics::clear_float4,
                love::graphics::clear_float3,
                love::graphics::clear
            ),
            "isActive", love::graphics::isActive,
            "origin", love::graphics::origin,
            "present", love::graphics::present,
            "getWidth", love::graphics::getWidth,
            "getHeight", love::graphics::getHeight
        ),
        "filesystem", luastate.create_table_with(
            "load", love::filesystem::load,
            "getInfo", love::filesystem::getInfo
        ),
        "timer", luastate.create_table_with(
            "sleep", love::timer::sleep,
            "step", love::timer::step,
            "getFPS", love::timer::getFPS
            /* 
            "getAverageDelta", love::timer::getAverageDelta,
            "getDelta", love::timer::getDelta */
        ),
        "system", luastate.create_table_with(
            "getOS", love::system::getOS
        ),
        "audio", luastate.create_table_with(
            "newSource", sol::overload(
                love::audio::newSource_file,
                love::audio::newSource_file_type
            )
        )
    );

    luastate.script(std::string(boot_lua, boot_lua + boot_lua_size));

    return 0;
}
