#pragma once

#include <sol/sol.hpp>
extern "C" {
    #include <lua.h>
}

namespace love {
    namespace timer {
        void    __init(sol::state &luastate);

        void    sleep(float ms);
        double  step();
        double  getFPS();
    }
}

int luaopen_love_timer(lua_State *L);