#pragma once

#include <sol/sol.hpp>
#include <vector>
#include <string>
extern "C" {
    #include <lua.h>
}

#include "classes/wiimoteController.hpp"
#include "classes/balanceBoard.hpp"

namespace love {
    namespace wiimote {
        void    __init(sol::state &luastate);
        void    __registerTypes(sol::state &luastate);

        WiimoteController* getWiimote(int index);
        sol::object        getWiimotes(sol::state &luastate);
        BalanceBoard*      getBalanceBoard();

        void    update();
    }
}

int luaopen_love_wiimote(lua_State *L);