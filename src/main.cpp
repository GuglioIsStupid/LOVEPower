#define SOL_ALL_SAFETIES_ON 1
// disable SOL_LUAJIT
#define SOL_LUAJIT 0

#include <sol/sol.hpp>
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <string>

#include "love/modules/graphics/classes/texture.hpp"
#include "love/modules/graphics/classes/font.hpp"

#include "love/love.hpp"
#include "love/modules/graphics/graphics.hpp"
#include "love/modules/filesystem/filesystem.hpp"
#include "love/modules/timer/timer.hpp"
#include "love/modules/system/system.hpp"
#include "love/modules/audio/audio.hpp"
#include "love/modules/math/math.hpp"
#include "love/modules/event/event.hpp"

#include "callbacks_lua.h"
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

    // Register love submodules
    love::graphics::__init(luastate);
    love::filesystem::__init(luastate, argc, argv);
    love::timer::__init(luastate);

    // Inject love table
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
            "draw", sol::overload(
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
        ),
        "system", luastate.create_table_with(
            "getOS", love::system::getOS,
            "getProcessorCount", love::system::getProcessorCount,
            "getPowerInfo", love::system::getPowerInfo
        ),
        "audio", luastate.create_table_with(
            "newSource", sol::overload(
                love::audio::newSource_file,
                love::audio::newSource_file_type
            )
        ),
        "math", luastate.create_table_with(
            "setRandomSeed", sol::overload(
                love::math::setRandomSeed_uint32,
                love::math::setRandomSeed_parts
            ),
            "random", sol::overload(
                love::math::random,
                love::math::random_max,
                love::math::random_min_max
            ),
            "setRandomState", love::math::setRandomState,
            "getRandomSeed", love::math::getRandomSeed,
            "getRandomState", love::math::getRandomState
        ),
        "event", luastate.create_table_with(
            "pump", love::event::pump,
            "poll", love::event::poll,
            "push", love::event::push,
            "quit", love::event::quit
        )
    );

    std::string callback_src(callbacks_lua, callbacks_lua + callbacks_lua_size);
    luastate["package"]["preload"]["love.callbacks"] = [callback_src](sol::this_state s) -> sol::object {
        lua_State* L = s;

        if (luaL_loadbuffer(L, callback_src.data(), callback_src.size(), "love.callbacks") != 0) {
            const char* msg = lua_tostring(L, -1);
            luaL_error(L, "failed to load love.callbacks: %s", msg ? msg : "unknown");
        }

        if (lua_pcall(L, 0, 1, 0) != 0) {
            const char* msg = lua_tostring(L, -1);
            luaL_error(L, "error running love.callbacks: %s", msg ? msg : "unknown");
        }

        sol::object ret = sol::stack::get<sol::object>(L, -1);
        lua_pop(L, 1); // clean up stack
        return ret;
    };


    luastate.script(std::string(boot_lua, boot_lua + boot_lua_size));

    return 0;
}
