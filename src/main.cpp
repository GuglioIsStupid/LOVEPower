#define SOL_ALL_SAFETIES_ON 1
// disable SOL_LUAJIT
#define SOL_LUAJIT 0

#include <sol/sol.hpp>
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <string>

#include "love/modules/graphics/classes/texture.hpp"
#include "love/modules/graphics/classes/font.hpp"

#include "love/modules/wiimote/classes/wiimoteController.hpp"

#include "love/love.hpp"
#include "love/modules/graphics/graphics.hpp"
#include "love/modules/filesystem/filesystem.hpp"
#include "love/modules/timer/timer.hpp"
#include "love/modules/system/system.hpp"
#include "love/modules/audio/audio.hpp"
#include "love/modules/math/math.hpp"
#include "love/modules/event/event.hpp"
#include "love/modules/wiimote/wiimote.hpp"
#include "love/modules/mii/miimodule.hpp"

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
        sol::lib::debug,
        sol::lib::io
    );

    // Register love submodules
    luastate["love"] = luastate.create_table();

    love::__init(luastate);
    love::audio::__init(luastate);
    love::event::__init(luastate);
    love::filesystem::__init(luastate, argc, argv);
    love::graphics::__init(luastate);
    love::math::__init(luastate);
    love::mii::__init(luastate);
    love::system::__init(luastate);
    love::timer::__init(luastate);
    love::wiimote::__init(luastate);

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

    try {
        luastate.script(std::string(boot_lua, boot_lua + boot_lua_size));
    }
    catch (const sol::error& e) {
        FILE* f = fopen("sd:/love_error_cpp.log", "w");
        if (f) {
            fprintf(f, "Lua boot failed:\n%s\n", e.what());
            fclose(f);
        }
        return -1;
    }

    exit(0);
    return 0;
}
