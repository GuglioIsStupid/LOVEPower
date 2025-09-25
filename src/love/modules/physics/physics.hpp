#pragma once

#include <sol/sol.hpp>
extern "C" {
    #include <lua.h>
}

namespace love {
    namespace physics {
        void __init(sol::state &luastate);
        void __registerTypes(sol::state &luastate);
    }
}

int luaopen_love_physics(lua_State *L);