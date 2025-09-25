#pragma once

#include <sol/sol.hpp>
#include "mii.h"
extern "C" {
    #include <lua.h>
}

namespace love {
    namespace mii {
        void    __init(sol::state &luastate);
        void    __registerTypes(sol::state &luastate);

        sol::object getMiis(sol::state &luastate);
    }
}

int luaopen_love_mii(lua_State *L);