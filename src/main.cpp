#define SOL_ALL_SAFETIES_ON 1
// disable SOL_LUAJIT
#define SOL_LUAJIT 0

#include <sol/sol.hpp>
#include <string>

#include "callbacks_lua.h"
#include "boot_lua.h"
#include "nogame_lua.h"

#include <grrlib.h>
#include "love/love.hpp"

inline void preloadLuaModule(sol::state& lua, const std::string& moduleName, const uint8_t* buffer, size_t size) {
    lua["package"]["preload"][moduleName] =
        [buffer, size, moduleName](sol::this_state s) -> sol::object {
            lua_State* L = s;

            if (luaL_loadbuffer(L, reinterpret_cast<const char*>(buffer), size, moduleName.c_str()) != 0) {
                const char* msg = lua_tostring(L, -1);
                luaL_error(L, "failed to load %s: %s", moduleName.c_str(), msg ? msg : "unknown");
            }

            if (lua_pcall(L, 0, 1, 0) != 0) {
                const char* msg = lua_tostring(L, -1);
                luaL_error(L, "error running %s: %s", moduleName.c_str(), msg ? msg : "unknown");
            }

            sol::object ret = sol::stack::get<sol::object>(L, -1);
            lua_pop(L, 1);
            return ret;
        };
}

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
        sol::lib::bit32,
        sol::lib::io,
        sol::lib::utf8
        //sol::lib::ffi // TODO: Implement LuaJit?
        //sol::lib::jit // Ditto
    );

    love::initialize(luastate, argc, argv);

    preloadLuaModule(luastate, "love.callbacks", callbacks_lua, callbacks_lua_size);
    preloadLuaModule(luastate, "love.nogame", nogame_lua, nogame_lua_size);

    try {
        luastate.script(std::string(boot_lua, boot_lua + boot_lua_size), "boot.lua", sol::load_mode::text);
    }
    catch (const sol::error& e) {
        FILE* f = fopen("sd:/love_error_cpp.log", "w");
        if (f) {
            fprintf(f, "Lua boot failed:\n%s\n", e.what());
            fclose(f);
        }
        return -1;
    }

    GRRLIB_Exit();
    std::exit(0);

    return 0;
}
